#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "hwutils.h"

int comparebystring(const void *a, const void *b)
{
    wordNode *ptr_a = *(wordNode **)a;
    wordNode *ptr_b = *(wordNode **)b;
    return strcmp(ptr_a->string, ptr_b->string);
}

int comparebycount(const void *a, const void *b)
{
    wordNode *ptr_a = *(wordNode **)a;
    wordNode *ptr_b = *(wordNode **)b;
    return ptr_a->count - ptr_b->count;
}

void ll_sortarray(wordNode *arr[], size_t count, char *type)
{
    if (0 == strcmp(type, "string"))
    {
        qsort(arr, count, sizeof(wordNode *), comparebystring);
    }
    else
    {
        qsort(arr, count, sizeof(wordNode *), comparebycount);
    }
}

wordNode **ll_getarray(wordNode *curWord)
{
    size_t count = ll_count(curWord);

    wordNode **arr = malloc(sizeof(wordNode *) * count);

    for (size_t i = 0; i < count; i++)
    {
        arr[i] = curWord;
        curWord = curWord->next;
    }

    return arr;
}

void ll_printarray(wordNode *arr[], size_t count, int perLine)
{
    for (size_t i = 0; i < count; i++)
    {
        printf("[%ld] %s:%ld", i, arr[i]->string, arr[i]->count);
        if (0 == (i + 1) % perLine)
        {
            printf("\n");
        }
        else
        {
            printf(" ");
        }
    }
}

int numberlength(int n)
{
    return log10(n) + 1;
}

void writearray(wordNode *arr[], size_t count)
{
    // find longest word // and "longest" count, in chars
    int longestWordLen = 0;
    int longestNumberLen = 0;
    for (size_t i = 0; i < count; i++)
    {
        int wordLen = strlen(arr[i]->string);
        if (wordLen > longestWordLen)
        {
            longestWordLen = wordLen;
        }

        int numberLen = numberlength(arr[i]->count);
        if (numberLen > longestNumberLen)
        {
            longestNumberLen = numberLen;
        }
    }

    char *sep = gettablesep(longestWordLen, longestNumberLen);

    printf("%s\n", sep);
    for (size_t i = 0; i < count; i++)
    {
        int countLen = numberlength(arr[i]->count);
        char countStr[16];
        snprintf(countStr, countLen + 1, "%ld", arr[i]->count);
        printf("%s\n", gettablerow(
                           arr[i]->string,
                           countStr,
                           strlen(arr[i]->string),
                           countLen,
                           longestWordLen,
                           longestNumberLen));
        printf("%s\n", sep);
    }
}

char *gettablerow(char *a_str, char *b_str, int a_len, int b_len, int a_max, int b_max)
{
    int a_right_pad = a_max - a_len;
    int b_right_pad = b_max - b_len;

    char *a_pad = malloc(sizeof(char) * a_right_pad + 1);
    for (int i = 0; i < a_right_pad; i++)
    {
        a_pad[i] = ' ';
    }
    a_pad[a_right_pad] = '\0';

    char *b_pad = malloc(sizeof(char) * b_right_pad + 1);
    for (int i = 0; i < b_right_pad; i++)
    {
        b_pad[i] = ' ';
    }
    b_pad[b_right_pad] = '\0';

    int strlen = 3 + a_max + b_max;

    char *result = malloc(sizeof(char) * strlen + 1);

    snprintf(result, strlen + 1, "|%s%s|%s%s|", a_str, a_pad, b_str, b_pad);

    result[strlen] = '\0';

    return result;
}

char *gettablesep(int a_max, int b_max)
{
    int strlen = 3 + a_max + b_max;
    char *result = malloc(sizeof(char) * strlen + 1);
    for (int i = 0; i < strlen; i++)
    {
        result[i] = '-';
    }
    result[strlen] = '\0';
    return result;
}

size_t ll_countword(wordNode **rootNode, char *word)
{
    size_t done = 0;
    size_t finalCount = 0;
    size_t totalWords = 0;
    wordNode *curNode = *rootNode;
    while (curNode->string && done == 0)
    {
        totalWords++;
        if (0 == strcmp(curNode->string, word))
        {
            curNode->count++;
            finalCount = curNode->count;
            done = 1;
        }
        if (curNode->next)
            curNode = curNode->next;
        else
            break;
    }
    if (done == 0)
    {
        wordNode *newWord = malloc(sizeof(wordNode));
        newWord->count = 1;
        newWord->next = NULL;
        int newStrLen = strlen(word);
        newWord->string = malloc(newStrLen + 1);
        strncpy(newWord->string, word, newStrLen);
        newWord->string[newStrLen] = '\0';
        if (totalWords == 0)
        {
            *rootNode = newWord;
        }
        else
        {
            curNode->next = newWord;
        }
        finalCount = newWord->count;
    }
    return finalCount;
}

size_t ll_getwordcount(wordNode *curWord, char *word)
{
    while (curWord)
    {
        if (0 == strcmp(curWord->string, word))
        {
            return curWord->count;
        }
        curWord = curWord->next;
    };

    return 0;
}

void ll_append(wordNode *word, char *str)
{
    while (word->next)
    {
        word = word->next;
    }
    wordNode *newWord = malloc(sizeof(wordNode));
    newWord->count = 1;
    newWord->next = NULL;
    int newStrLen = strlen(str);
    newWord->string = malloc(newStrLen + 1);
    strncpy(newWord->string, str, newStrLen);
    newWord->string[newStrLen] = '\0';
    word->next = newWord;
}

void ll_deletelast(wordNode *word)
{
    wordNode *prevWord = word;
    while (word->next)
    {
        prevWord = word;
        word = word->next;
    }
    prevWord->next = NULL;
    free(word);
}

void ll_print(wordNode *word)
{
    size_t i = 0;
    while (word)
    {
        printf("%zu: \"%s\" (%lu)\n", i, word->string, word->count);
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
    if (!word->string)
    {
        return 0;
    }
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
    int start_index = -1;
    int end_index = -1;
    int minlength = 2;

    wordNode *root = malloc(sizeof(wordNode));
    root->count = 0;
    root->string = NULL;
    root->next = NULL;

    wordNode *prevNode;
    int firstWord = 1;

    for (i = 0; i < len; i++)
    {
        // printf("[%ld]: %c\n", i, line[i]);
        if (isletter(line[i]) && i != len - 1)
        {
            if (start_index == -1)
            {
                start_index = i;
                // printf("  Setting start_index to %ld\n", i);
            }
        }
        else
        {
            if (start_index != -1)
            {
                if (i == len - 1 && isletter(line[i]))
                {
                    end_index = i;
                }
                else
                {
                    end_index = i - 1;
                }
                // printf("  Setting end_index to %ld\n", end_index);
                if ((end_index - start_index + 1) > minlength)
                {
                    int strSize = end_index - start_index + 1;
                    char *newStr = malloc(sizeof(char) * (strSize + 1));
                    strncpy(newStr, line + start_index, strSize);
                    newStr[strSize] = '\0';
                    // printf("  New word: [%s]\n", newStr);
                    wordNode *newNode = malloc(sizeof(wordNode));
                    newNode->count = 1;
                    newNode->next = NULL;
                    newNode->string = newStr;
                    if (!firstWord)
                    {
                        // printf("Setting prevNode->next\n");
                        prevNode->next = newNode;
                    }
                    else
                    {
                        firstWord = 0;
                        // printf("Setting root\n");
                        root = newNode;
                    }
                    prevNode = newNode;
                }
                start_index = -1;
                // printf("  Current words: %s\n", ll_implode(root, '|'));
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
