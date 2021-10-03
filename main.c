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

    fp = fopen("testfile2", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    wordNode *mainList = malloc(sizeof(wordNode));
    mainList->string = "\0";
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
    ll_print(mainList);
    wordNode **mainListArray = ll_getarray(mainList);
    size_t arrLen = ll_count(mainList);

    printf("Array:\n");
    ll_printarray(mainListArray, arrLen, 10);

    printf("Size: %lu\n", arrLen);
    printf("Sort by word:\n");
    ll_sortarray(mainListArray, arrLen, "string");
    ll_printarray(mainListArray, arrLen, 10);

    printf("Sort by count:\n");
    ll_sortarray(mainListArray, arrLen, "count");
    ll_printarray(mainListArray, arrLen, 10);

    fclose(fp);
    if (line)
        free(line);

    exit(EXIT_SUCCESS);
}
