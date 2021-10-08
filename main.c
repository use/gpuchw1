#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include "hwutils.h"
#include "timing.h"

typedef struct timeStats
{
    double word_count;
    double list_merge;
    double total_without_io;
} timeStats;

timeStats *run_program(
    int numThreads,
    int numMergingThreads,
    char **textLines,
    size_t numLines,
    char *fileNamePrefix);

int main(void)
{
    int numlines;
    char *line = NULL;
    FILE *fp;
    size_t len = 0;
    char *inFileName = "testfile2";
    char *textLines[99999];

    for (int i = 0; i < 99999; i++)
    {
        textLines[i] = NULL;
    }
    fp = fopen(inFileName, "r");
    if (fp == NULL)
    {
        exit(EXIT_FAILURE);
    }

    // read in lines
    size_t lineCounter = 0;
    while (true)
    {
        len = getline(&textLines[lineCounter], &len, fp);
        if (-1 == len)
        {
            break;
        }
        lowercase(textLines[lineCounter], len);
        lineCounter++;
    }

    fclose(fp);
    if (line)
    {
        free(line);
    }

    double t1 = 0;
    double t2 = 0;
    char fileNamePrefix[256];

    int num_count_threads = 4;
    int num_merge_threads = 2;

    printf("================ Run 1 ================\n");
    printf("(1 counting thread, 1 merging thread)\n");
    snprintf(fileNamePrefix, 256, "%s_%d_%s", inFileName, 1, "thread");
    timeStats *stats_1 = run_program(1, 1, textLines, lineCounter, fileNamePrefix);
    printf("%%%%%% Time: %lf\n", stats_1->total_without_io);

    printf("\n================ Run 2 ================\n");
    printf("(%d counting threads, 1 merging thread)\n", num_count_threads);
    snprintf(fileNamePrefix, 256, "%s_%d_%s", inFileName, num_count_threads, "threads");
    timeStats *stats_2 = run_program(num_count_threads, 1, textLines, lineCounter, fileNamePrefix);
    printf("%%%%%% Time: %lf\n", stats_2->total_without_io);

    printf("\n================ Run 3 ================\n");
    printf("(%d counting threads, %d merging threads)\n", num_count_threads, num_merge_threads);
    snprintf(fileNamePrefix, 256, "%s_%d_%s", inFileName, num_count_threads, "threads_parallel_merge");
    timeStats *stats_3 = run_program(num_count_threads, num_merge_threads, textLines, lineCounter, fileNamePrefix);
    printf("%%%%%% Time: %lf\n", stats_3->total_without_io);

    printf("\n================ Timing Results ================\n");
    printf("%%%%%% The speedup of counting with %d threads: %lf\n",
           num_count_threads,
           stats_1->word_count / stats_2->word_count);
    printf("%%%%%% The efficiency of counting with %d threads: %lf\n",
           num_count_threads,
           stats_1->word_count / stats_2->word_count / num_count_threads);
    printf("%%%%%% The speedup of the merge operation with %d threads: %lf\n",
           num_merge_threads,
           stats_2->list_merge / stats_3->list_merge);
    printf("%%%%%% The efficiency of the merge operation with %d threads: %lf\n",
           num_merge_threads,
           stats_2->list_merge / stats_3->list_merge / num_merge_threads);

    exit(EXIT_SUCCESS);
}

timeStats *run_program(
    int numThreads,
    int numMergingThreads,
    char **textLines,
    size_t numLines,
    char *fileNamePrefix)
{
    double count_start = 0;
    double count_end = 0;
    double merge_start = 0;
    double merge_end = 0;

    // create threads
    count_start = currentTime();
    wordNode *results[numThreads];
    for (int i = 0; i < numThreads; i++)
    {
        results[i] = malloc(sizeof(wordNode));
        results[i]->count = 0;
        results[i]->next = NULL;
        results[i]->string = NULL;
    }
    pthread_t threads[numThreads];
    size_t linesPerThread = ceil((float)numLines / (float)numThreads);
    printf("Total lines: %ld\n", numLines);
    printf("Threads: %d\n", numThreads);
    printf("Lines per thread: %ld\n", linesPerThread);
    printf("Beginning count (%d threads)...\n", numThreads);

    for (int i = 0; i < numThreads; i++)
    {
        jobSpec *js = malloc(sizeof(jobSpec));
        js->lines = textLines;
        js->lines_len = numLines;
        js->max = linesPerThread;
        js->results = results;
        js->results_len = numThreads;
        js->start = i * linesPerThread;
        js->thread_id = i;
        if (pthread_create(&(threads[i]), NULL, workerThread, js) != 0)
        {
            printf("Couldn't create thread\n");
        }
    }

    for (int i = 0; i < numThreads; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            printf("Couldn't join thread\n");
        }
    }

    count_end = currentTime();

    for (int i = 0; i < numThreads; i++)
    {
        size_t count = ll_count(results[i]);
        printf("[Thread %d]: %ld unique words\n", i, count);
    }

    // merge lists (threaded)
    merge_start = currentTime();
    int numListsRemaining = numThreads;
    printf("Beginning list merge (%d threads)...\n", numMergingThreads);

    bool listCompletionTracker[numThreads];
    for (int i = 0; i < numThreads; i++)
    {
        listCompletionTracker[i] = false;
    }
    bool threadStatusTracker[numMergingThreads];
    for (int i = 0; i < numMergingThreads; i++)
    {
        threadStatusTracker[i] = false;
    }
    pthread_t mergeThreads[numMergingThreads];
    while (numListsRemaining > 1)
    {
        size_t starting_index = 0;
        bool reached_end = false;

        for (int i = 0; i < numMergingThreads && !reached_end; i++)
        {
            // thread setup
            signed long dest_index = next_false_value(listCompletionTracker, starting_index, numThreads);
            starting_index = dest_index + 1;
            signed long src_index = next_false_value(listCompletionTracker, starting_index, numThreads);
            starting_index = src_index + 1;

            if (dest_index != -1 && src_index != -1)
            {
                listCompletionTracker[src_index] = true;

                mergeJobSpec *js = malloc(sizeof(mergeJobSpec));
                js->dest = results[dest_index];
                js->src = results[src_index];
                printf("[Thread %d] merge list %ld -> %ld\n", i, src_index, dest_index);
                if (pthread_create(&(mergeThreads[i]), NULL, mergeWorkerThread, js) != 0)
                {
                    printf("Couldn't create thread\n");
                }
                threadStatusTracker[i] = true;
                numListsRemaining -= 1;
            }
            else
            {
                reached_end = true;
            }
        }

        for (int i = 0; i < numMergingThreads; i++)
        {
            if (threadStatusTracker[i])
            {
                if (pthread_join(mergeThreads[i], NULL) != 0)
                {
                    printf("Couldn't join thread\n");
                }
                threadStatusTracker[i] = false;
            }
        }

        printf("[");
        for (size_t i = 0; i < numThreads; i++)
        {
            char *status = listCompletionTracker[i] ? "+" : " ";
            printf("%s", status);
        }
        printf("]");
        printf("\n");
    }

    merge_end = currentTime();

    wordNode **mainListArray = ll_getarray(results[0]);
    size_t arrLen = ll_count(results[0]);

    printf("Total unique words: %lu\n", arrLen);
    printf("Sort by word...\n");
    ll_sortarray(mainListArray, arrLen, "string");

    char sort_by_word_suffix[] = "_sort_by_word.txt";
    char out_file_name_by_word[256];
    snprintf(out_file_name_by_word, 256, "%s%s", fileNamePrefix, sort_by_word_suffix);
    FILE *fp_out_by_word = fopen(out_file_name_by_word, "w");
    writearray(fp_out_by_word, mainListArray, arrLen);
    fclose(fp_out_by_word);
    printf("\tWrote %s\n", out_file_name_by_word);

    printf("Sort by count...\n");
    ll_sortarray(mainListArray, arrLen, "count");

    char *sort_by_count_suffix = "_sort_by_count.txt";
    char out_file_name_by_count[256];
    snprintf(out_file_name_by_count, 256, "%s%s", fileNamePrefix, sort_by_count_suffix);
    FILE *fp_out_by_count = fopen(out_file_name_by_count, "w");
    writearray(fp_out_by_count, mainListArray, arrLen);
    fclose(fp_out_by_count);
    printf("\tWrote %s\n", out_file_name_by_count);

    timeStats *stats = malloc(sizeof(timeStats));
    stats->word_count = count_end - count_start;
    stats->list_merge = merge_end - merge_start;
    stats->total_without_io = stats->word_count + stats->list_merge;

    return stats;
}
