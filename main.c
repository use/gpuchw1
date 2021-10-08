#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include "hwutils.h"
#include "timing.h"

int do_the_thing(
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

    printf("================ Run 1 ================\n");
    snprintf(fileNamePrefix, 256, "%s%s", inFileName, "_1_thread_");
    t1 = currentTime();
    do_the_thing(1, 1, textLines, lineCounter, fileNamePrefix);
    t2 = currentTime();
    double cost_1 = t2 - t1;
    printf("%%%%%% Time: %lf\n", cost_1);

    printf("\n================ Run 2 ================\n");
    snprintf(fileNamePrefix, 256, "%s%s", inFileName, "_4_threads_");
    t1 = currentTime();
    do_the_thing(4, 1, textLines, lineCounter, fileNamePrefix);
    t2 = currentTime();
    double cost_2 = t2 - t1;
    printf("%%%%%% Time: %lf\n", cost_2);

    printf("\n================ Run 3 ================\n");
    snprintf(fileNamePrefix, 256, "%s%s", inFileName, "_4_threads_parallel_merge_");
    t1 = currentTime();
    do_the_thing(4, 2, textLines, lineCounter, fileNamePrefix);
    t2 = currentTime();
    double cost_3 = t2 - t1;
    printf("%%%%%% Time: %lf\n", cost_3);

    printf("\n================ Timing Results ================\n");
    printf("%%%%%% The speedup (SerialTimeCost / ParallelTimeCost) when using 4 threads is %lf\n", cost_1 / cost_2);
    printf("%%%%%% The efficiency (Speedup / NumProcessorCores) when using 4 threads is %lf\n", cost_1 / cost_2 / 4);
    printf("%%%%%% The overall speedup when using 2 threads to merge lists is %lf\n", cost_2 / cost_3);

    exit(EXIT_SUCCESS);
}

int do_the_thing(
    int numThreads,
    int numMergingThreads,
    char **textLines,
    size_t numLines,
    char *fileNamePrefix)
{
    // create threads
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
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < numThreads; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < numThreads; i++)
    {
        size_t count = ll_count(results[i]);
        printf("[thread %d]: %ld unique words\n", i, count);
    }

    // merge lists (threaded)
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
            // thread a
            signed long a_dest_index = next_false_value(listCompletionTracker, starting_index, numThreads);
            starting_index = a_dest_index + 1;
            signed long a_src_index = next_false_value(listCompletionTracker, starting_index, numThreads);
            starting_index = a_src_index + 1;

            if (a_dest_index != -1 && a_src_index != -1)
            {
                listCompletionTracker[a_src_index] = true;

                mergeJobSpec *js_a = malloc(sizeof(mergeJobSpec));
                js_a->dest = results[a_dest_index];
                js_a->src = results[a_src_index];
                printf("[Thread %d] merge list %ld -> %ld\n", i, a_src_index, a_dest_index);
                if (pthread_create(&(mergeThreads[i]), NULL, mergeWorkerThread, js_a) != 0)
                {
                    return EXIT_FAILURE;
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
                    return EXIT_FAILURE;
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
}
