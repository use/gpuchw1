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

    fp = fopen("testfile1", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    wordNode *mainList = malloc(sizeof(wordNode));

    while ((len = getline(&line, &len, fp)) != -1)
    {
        lowercase(line, len);
        wordNode *curNode = tokenize(line, strlen(line));
        while (curNode)
        {
            size_t count = ll_countword(&mainList, curNode->string);
            curNode = curNode->next;
        }

        printf("Length: %zu:\n", len);
        printf("%s\n", line);
        lowercase(line, len);
        printf("%s\n", line);
    }
    ll_print(mainList);

    fclose(fp);
    if (line)
        free(line);

    exit(EXIT_SUCCESS);
}
