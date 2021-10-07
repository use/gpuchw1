#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include "hwutils.h"

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

    // create threads
    int numThreads = 6;
    wordNode *results[numThreads];
    for (int i = 0; i < numThreads; i++)
    {
        results[i] = malloc(sizeof(wordNode));
        results[i]->count = 0;
        results[i]->next = NULL;
        results[i]->string = NULL;
    }
    pthread_t threads[numThreads];
    size_t linesPerThread = ceil((float)lineCounter / (float)numThreads);
    printf("Total lines: %ld\n", lineCounter);
    printf("Threads: %d\n", numThreads);
    printf("Lines per thread: %ld\n", linesPerThread);

    for (int i = 0; i < numThreads; i++)
    {
        jobSpec *js = malloc(sizeof(jobSpec));
        js->lines = textLines;
        js->lines_len = lineCounter;
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

    printf("Beginning list merge...\n");

    // merge lists (threaded)
    int numListsRemaining = numThreads;
    int numMergingThreads = 2;
    bool listCompletionTracker[numThreads];
    for (int i = 0; i < numThreads; i++)
    {
        listCompletionTracker[i] = false;
    }
    pthread_t mergeThreads[numMergingThreads];
    while (numListsRemaining > 1)
    {
        size_t starting_index = 0;

        // thread a
        size_t a_dest_index = next_false_value(listCompletionTracker, starting_index, numThreads);
        starting_index = a_dest_index + 1;
        size_t a_src_index = next_false_value(listCompletionTracker, starting_index, numThreads);
        starting_index = a_src_index + 1;

        listCompletionTracker[a_src_index] = true;

        mergeJobSpec *js_a = malloc(sizeof(mergeJobSpec));
        js_a->dest = results[a_dest_index];
        js_a->src = results[a_src_index];
        printf("[Thread 1] merge list %ld -> %ld\n", a_src_index, a_dest_index);
        if (pthread_create(&(mergeThreads[0]), NULL, mergeWorkerThread, js_a) != 0)
        {
            return EXIT_FAILURE;
        }
        numListsRemaining -= 1;

        if (numListsRemaining > 2)
        {
            // thread b
            size_t b_dest_index = next_false_value(listCompletionTracker, starting_index, numThreads);
            starting_index = b_dest_index + 1;
            size_t b_src_index = next_false_value(listCompletionTracker, starting_index, numThreads);
            // starting_index = b_src_index + 1; // not needed

            listCompletionTracker[b_src_index] = true;

            mergeJobSpec *js_b = malloc(sizeof(mergeJobSpec));
            js_b->dest = results[b_dest_index];
            js_b->src = results[b_src_index];
            printf("[Thread 2] merge list %ld -> %ld\n", b_src_index, b_dest_index);
            if (pthread_create(&(mergeThreads[1]), NULL, mergeWorkerThread, js_b) != 0)
            {
                return EXIT_FAILURE;
            }
            numListsRemaining -= 1;

            if (pthread_join(mergeThreads[1], NULL) != 0)
            {
                return EXIT_FAILURE;
            }
        }

        if (pthread_join(mergeThreads[0], NULL) != 0)
        {
            return EXIT_FAILURE;
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

    char sort_by_word_suffix[] = "_by_word.txt";
    char out_file_name_by_word[256];
    snprintf(out_file_name_by_word, 256, "%s%s", inFileName, sort_by_word_suffix);
    FILE *fp_out_by_word = fopen(out_file_name_by_word, "w");
    writearray(fp_out_by_word, mainListArray, arrLen);
    fclose(fp_out_by_word);
    printf("\tWrote %s\n", out_file_name_by_word);

    printf("Sort by count...\n");
    ll_sortarray(mainListArray, arrLen, "count");

    char *sort_by_count_suffix = "_by_count.txt";
    char out_file_name_by_count[256];
    snprintf(out_file_name_by_count, 256, "%s%s", inFileName, sort_by_count_suffix);
    FILE *fp_out_by_count = fopen(out_file_name_by_count, "w");
    writearray(fp_out_by_count, mainListArray, arrLen);
    fclose(fp_out_by_count);
    printf("\tWrote %s\n", out_file_name_by_count);

    fclose(fp);
    if (line)
        free(line);

    exit(EXIT_SUCCESS);
}
