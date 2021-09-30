#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(void)
{
    int numlines;
    char *line = NULL;
    FILE *fp;
    __ssize_t len;

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

    char* words[] = {"a", "bc", "def"};
    size_t numwords = sizeof(words) / sizeof(words[0]);
    implode(words, numwords);

    exit(EXIT_SUCCESS);
}

char **tokenize(char *line, size_t len)
{
    for (int i = 0; i < len; i++)
    {
    }
}

void implode(char** words, size_t numwords)
{
    size_t wordlengthsum = 0;
    for (size_t i = 0; i < numwords; i++)
    {
        wordlengthsum += strlen(words[i]);
    }

    size_t newlength = wordlengthsum + numwords - 1;
    char result[newlength + 1];

    printf("strlen(words[i]): %d\n", strlen(words[0]));
    printf("numwords: %d\n", numwords);
    printf("newlength: %d\n", newlength);
    printf("wordlengthsum: %d\n", wordlengthsum);

    size_t index = 0;
    for (size_t i = 0; i < numwords; i++)
    {
        for (size_t j = 0; j < strlen(words[i]); j++)
        {
            result[index] = words[i][j];
            index++;
        }
        if (index < newlength)
        {
            result[index] = ',';
            index++;
        }
    }
    result[newlength + 1] = '\0';

    printf("imploded: %s", result);
}

void lowercase(char *chars, size_t len)
{
    for (int i = 0; i < len; i++)
    {
        chars[i] = tolower(chars[i]);
    }
}
