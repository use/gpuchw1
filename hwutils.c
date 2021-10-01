#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

char **tokenize(char *line, size_t len)
{
    size_t i;
    size_t start_index = -1;

    for (i = 0; i < len; i++)
    {
        if (isletter(line[i]))
        {
            if (start_index < 0) {
                start_index = i;
            } else {

            }
        } else {
            end_index = i;
            // copystring(line, dest, start_index, end_index);
        }

    }
}

bool isletter(char c)
{
    int ascii = (int)c;
    if (
        ascii >= 65 && ascii <= 90 ||
        ascii >= 97 && ascii <= 122)
        return true;

    return false;
}

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
