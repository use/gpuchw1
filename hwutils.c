#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "hwutils.h"

void ll_print(wordNode *word)
{
    while (word)
    {
        printf("%s\n", word->string);
        word = word->next;
    }
}

wordNode *tokenize(char *line, size_t len)
{
    size_t i;
    size_t start_index = -1;
    size_t end_index = -1;

    wordNode *root;
    wordNode *curNode;

    for (i = 0; i < len; i++)
    {
        if (isletter(line[i]))
        {
            if (start_index == -1)
            {
                start_index = i;
            }
        }
        else
        {
            if (start_index != -1)
            {
                end_index = i;
                char newStr[end_index - start_index];
                strncpy(newStr, line + start_index, end_index - start_index);
                wordNode *newNode;
                newNode->string = newStr;
                if (curNode)
                {
                    curNode->next = newNode;
                }
                else
                {
                    root = newNode;
                }
                curNode = newNode;
            }
        }
    }

    return root;
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

char *ll_implode(wordNode *word, char sep)
{
    size_t wordlengthsum = 0;
    wordNode *tmp_word = word;
    size_t numwords = 0;
    while (tmp_word)
    {
        numwords++;
        wordlengthsum += strlen(tmp_word->string);
        tmp_word = tmp_word->next;
    }

    size_t newlength = wordlengthsum + numwords - 1;
    char *result = malloc(sizeof(char) * (newlength + 1));

    printf("numwords: %zu\n", numwords);
    printf("newlength: %zu\n", newlength);
    printf("wordlengthsum: %zu\n", wordlengthsum);

    size_t index = 0;
    tmp_word = word;
    while (tmp_word)
    {
        for (size_t j = 0; j < strlen(tmp_word->string); j++)
        {
            result[index] = tmp_word->string[j];
            index++;
        }
        if (index < newlength)
        {
            result[index] = sep;
            index++;
        }
        tmp_word = tmp_word->next;
    }
    result[newlength] = '\0';

    return result;
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
