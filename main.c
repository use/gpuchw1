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

    while ((len = getline(&line, &len, fp)) != -1)
    {
        printf("Length: %zu:\n", len);
        printf("%s\n", line);
        lowercase(line, len);
        printf("%s\n", line);
    }

    fclose(fp);
    if (line)
        free(line);

    char *words[] = {"a", "bc", "def"};
    size_t numwords = sizeof(words) / sizeof(words[0]);
    char *imploded = implode(words, numwords);
    printf("imploded: %s\n", imploded);

    exit(EXIT_SUCCESS);
}
