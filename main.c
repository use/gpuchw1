#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "hwutils.h"

int main(void)
{
    int numlines;
    char *line = NULL;
    FILE *fp;
    size_t len;
    char *inFileName = "testfile4";

    fp = fopen(inFileName, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    wordNode *mainList = malloc(sizeof(wordNode));
    mainList->string = NULL;
    mainList->next = NULL;
    mainList->count = 0;

    size_t lineCounter = 0;
    while ((len = getline(&line, &len, fp)) != -1)
    {
        lineCounter++;
        lowercase(line, len);
        // printf("[%lu] Line Length: %zu:\n", lineCounter, len);
        // printf("\"%s\"\n", line);

        wordNode *curNode = tokenize(line, strlen(line));
        // ignore blank lines
        if (!curNode->string)
        {
            // printf("Skipping line\n");
            continue;
        }
        while (curNode)
        {
            // printf("Word: %s\n", curNode->string);
            size_t count = ll_countword(&mainList, curNode->string);
            curNode = curNode->next;
        }
    }
    wordNode **mainListArray = ll_getarray(mainList);
    size_t arrLen = ll_count(mainList);

    printf("Size: %lu\n", arrLen);
    printf("Sort by word:\n");
    ll_sortarray(mainListArray, arrLen, "string");

    char sort_by_word_suffix[] = "_by_word.txt";
    char out_file_name_by_word[256];
    snprintf(out_file_name_by_word, 256, "%s%s", inFileName, sort_by_word_suffix);
    FILE *fp_out_by_word = fopen(out_file_name_by_word, "w");
    writearray(fp_out_by_word, mainListArray, arrLen);
    fclose(fp_out_by_word);

    printf("Sort by count:\n");
    ll_sortarray(mainListArray, arrLen, "count");

    char *sort_by_count_suffix = "_by_count.txt";
    char out_file_name_by_count[256];
    snprintf(out_file_name_by_count, 256, "%s%s", inFileName, sort_by_count_suffix);
    FILE *fp_out_by_count = fopen(out_file_name_by_count, "w");
    writearray(fp_out_by_count, mainListArray, arrLen);
    fclose(fp_out_by_count);

    fclose(fp);
    if (line)
        free(line);

    exit(EXIT_SUCCESS);
}
