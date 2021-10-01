#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "hwutils.h"

void ll_print(wordNode *word)
{
    size_t i = 0;
    while (word)
    {
        printf("%zu: %s\n", i, word->string);
        word = word->next;
        i++;
    }
}

wordNode *ll_lastnode(wordNode *word)
{
    while (word->next)
    {
        word = word->next;
    }
    wordNode *lastWord = word;
    return lastWord;
}

wordNode *ll_nodeatindex(wordNode *word, size_t n)
{
    size_t index = 0;
    while (index < n && word->next)
    {
        word = word->next;
        index++;
    }
    return word;
}

size_t ll_count(wordNode *word)
{
    size_t count = 0;
    while (word)
    {
        count++;
        word = word->next;
    }
    return count;
}

wordNode *tokenize(char *line, size_t len)
{
    size_t i;
    size_t start_index = -1;
    size_t end_index = -1;

    wordNode *root;
    wordNode *prevNode;
    int firstWord = 1;

    for (i = 0; i < len; i++)
    {
        printf("[%ld]: %c\n", i, line[i]);
        if (isletter(line[i]) && i != len - 1)
        {
            if (start_index == -1)
            {
                start_index = i;
                printf("  Setting start_index to %ld\n", i);
            }
        }
        else
        {
            if (start_index != -1)
            {
                if (i == len - 1) {
                    end_index = i;
                }
                else
                {
                    end_index = i - 1;
                }
                printf("  Setting end_index to %ld\n", end_index);
                char *newStr = malloc(sizeof(char) * (end_index - start_index + 1));
                strncpy(newStr, line + start_index, end_index - start_index + 1);
                printf("  New word: [%s]\n", newStr);
                wordNode *newNode = malloc(sizeof(wordNode));
                newNode->string = newStr;
                if (!firstWord)
                {
                    printf("Setting prevNode->next\n");
                    prevNode->next = newNode;
                }
                else
                {
                    firstWord = 0;
                    printf("Setting root\n");
                    root = newNode;
                }
                prevNode = newNode;
                start_index = -1;
                printf("  Current words: %s\n", ll_implode(root, '|'));
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
