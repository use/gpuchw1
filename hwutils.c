#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

// char **tokenize(char *line, size_t len)
// {
//     for (int i = 0; i < len; i++)
//     {
//     }
// }

char *implode(char **words, size_t numwords)
{
    size_t wordlengthsum = 0;
    for (size_t i = 0; i < numwords; i++)
    {
        wordlengthsum += strlen(words[i]);
    }

    size_t newlength = wordlengthsum + numwords - 1;
    char *result = malloc(sizeof(char) * (newlength + 1));

    printf("numwords: %zu\n", numwords);
    printf("newlength: %zu\n", newlength);
    printf("wordlengthsum: %zu\n", wordlengthsum);

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
    result[newlength] = '\0';

    return result;
}

void lowercase(char *chars, size_t len)
{
    for (int i = 0; i < len; i++)
    {
        chars[i] = tolower(chars[i]);
    }
}
