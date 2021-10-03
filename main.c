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

    fp = fopen("testfile4", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    wordNode *mainList = malloc(sizeof(wordNode));

    size_t lineCounter = 0;
    while ((len = getline(&line, &len, fp)) != -1)
    {
        lineCounter++;
        lowercase(line, len);
        printf("[%lu] Line Length: %zu:\n", lineCounter, len);
        printf("\"%s\"\n", line);

        wordNode *curNode = tokenize(line, strlen(line));
        printf("First word: \"%s\"\n", curNode->string);
        // ignore blank lines
        if (!curNode->string)
        {
            printf("Skipping line\n");
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
    ll_printarray(mainListArray, arrLen);

    printf("Size: %lu\n", arrLen);
    printf("Sort by word:\n");
    ll_sortarray(mainListArray, arrLen, "string");
    ll_printarray(mainListArray, arrLen);

    printf("Sort by count:\n");
    ll_sortarray(mainListArray, arrLen, "count");
    ll_printarray(mainListArray, arrLen);

    fclose(fp);
    if (line)
        free(line);

    exit(EXIT_SUCCESS);
}
