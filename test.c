#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "hwutils.h"

int main(void)
{
    wordNode *firstWord = malloc(sizeof(wordNode));
    firstWord->string = "hello";

    wordNode *secondWord = malloc(sizeof(wordNode));
    secondWord->string = "there";

    wordNode *thirdWord = malloc(sizeof(wordNode));
    thirdWord->string = "again";

    firstWord->next = secondWord;
    secondWord->next = thirdWord;

    ll_print(firstWord);

    printf("%s\n", ll_implode(firstWord, '|'));
    assert(0 == strcmp("hello|there|again", ll_implode(firstWord, '|')));
    assert(0 == strcmp("again", ll_implode(thirdWord, '|')));

    printf("count: %ld\n", ll_count(firstWord));
    assert(3 == ll_count(firstWord));

    wordNode *lastWord = ll_lastnode(firstWord);
    printf("last word: %s\n", lastWord->string);

    assert(!isletter(','));
    assert(!isletter('8'));
    assert(!isletter(' '));
    assert(isletter('A'));
    assert(isletter('f'));
    assert(isletter('z'));
    assert(isletter('Z'));

    char *testLineDeleteLast = "white blue black red green";
    wordNode *colorList = tokenize(testLineDeleteLast, strlen(testLineDeleteLast));
    assert(5 == ll_count(colorList));
    ll_deletelast(colorList);
    assert(4 == ll_count(colorList));
    assert(0 == strcmp("white|blue|black|red", ll_implode(colorList, '|')));
    ll_append(colorList, "purple");
    ll_append(colorList, "yellow");
    assert(6 == ll_count(colorList));
    assert(0 == strcmp("white|blue|black|red|purple|yellow", ll_implode(colorList, '|')));

    char *line = "it was a bright cold day in april";
    wordNode *tokenized = tokenize(line, strlen(line));
    printf("count: %ld\n", ll_count(tokenized));
    ll_print(tokenized);

    assert(0 == strcmp(tokenized->string, "it"));

    wordNode *wordat1 = ll_nodeatindex(tokenized, 1);

    printf("Word at 1: %s\n", wordat1->string);
    printf("Word at -1: %s\n", ll_nodeatindex(tokenized, -1)->string);
    printf("Word at 0: %s\n", ll_nodeatindex(tokenized, 0)->string);
    printf("Word at 111: %s\n", ll_nodeatindex(tokenized, 111)->string);
    printf("Word at 7: %s\n", ll_nodeatindex(tokenized, 7)->string);

    size_t numLines = 3;
    char *lines[numLines];
    lines[0] = "hello hello hello and welcome to my channel";
    lines[1] = "hello and welcome to my webpage";
    lines[2] = "hey there welcome... to my webpage...";

    wordNode *tokenLists[numLines];
    for (size_t i = 0; i < 3; i++)
    {
        tokenLists[i] = tokenize(lines[i], strlen(lines[i]));
        printf("%s\n", ll_implode(tokenLists[i], '_'));
    }

    wordNode *mainList = malloc(sizeof(wordNode));
    for (size_t i = 0; i < numLines; i++)
    {
        wordNode *curNode = tokenLists[i];
        while (curNode)
        {
            size_t count = ll_countword(&mainList, curNode->string);
            curNode = curNode->next;
        }
    }
    ll_print(mainList);

    assert(4 == ll_getwordcount(mainList, "hello"));
    assert(0 == ll_getwordcount(mainList, "asdfasdf"));

    char *anotherLine = "red orange yellow green blue purple";
    wordNode *anotherList = tokenize(anotherLine, strlen(anotherLine));
    printf("word: %s\n", ll_implode(anotherList, ','));
    wordNode **arr = ll_getarray(anotherList);
    size_t arrCount = ll_count(anotherList);
    ll_printarray(arr, arrCount);
    ll_sortarray(arr, arrCount, "string");
    printf("Sorted by string alphabetically:\n");
    ll_printarray(arr, arrCount);

    wordNode **mainListArr = ll_getarray(mainList);
    size_t mainListCount = ll_count(mainList);
    printf("List of words:\n");
    ll_printarray(mainListArr, mainListCount);
    printf("Sorted by string:\n");
    ll_sortarray(mainListArr, mainListCount, "string");
    ll_printarray(mainListArr, mainListCount);
    assert(0 == strcmp(mainListArr[0]->string, "and"));
    assert(0 == strcmp(mainListArr[8]->string, "welcome"));
    printf("Sorted by count:\n");
    ll_sortarray(mainListArr, mainListCount, "count");
    assert(0 == strcmp(mainListArr[0]->string, "channel"));
    assert(0 == strcmp(mainListArr[8]->string, "hello"));
    assert(4 == mainListArr[8]->count);
    ll_printarray(mainListArr, mainListCount);

    char *newLineTestText = "asdf,";
    wordNode *newLineTestTokens = tokenize(newLineTestText, strlen(newLineTestText));
    assert(0 == strcmp(ll_lastnode(newLineTestTokens)->string, "asdf"));
    ll_print(newLineTestTokens);
}
