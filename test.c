#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <pthread.h>
#include "hwutils.h"

int main(void)
{
    wordNode *firstWord = malloc(sizeof(wordNode));
    firstWord->string = "hello";
    firstWord->count = 1;
    firstWord->next = NULL;

    wordNode *secondWord = malloc(sizeof(wordNode));
    secondWord->string = "there";
    secondWord->count = 1;
    firstWord->next = secondWord;

    wordNode *thirdWord = malloc(sizeof(wordNode));
    thirdWord->string = "again";
    thirdWord->count = 1;
    thirdWord->next = NULL;
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

    assert(0 == strcmp(tokenized->string, "was"));

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
    mainList->string = NULL;
    mainList->next = NULL;
    mainList->count = 0;
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
    assert(6 == arrCount);
    ll_printarray(arr, arrCount, 1);
    ll_sortarray(arr, arrCount, "string");
    printf("Sorted by string alphabetically:\n");
    ll_printarray(arr, arrCount, 1);

    wordNode **mainListArr = ll_getarray(mainList);
    size_t mainListCount = ll_count(mainList);
    printf("List of words:\n");
    ll_printarray(mainListArr, mainListCount, 1);
    printf("Sorted by string:\n");
    ll_sortarray(mainListArr, mainListCount, "string");
    ll_printarray(mainListArr, mainListCount, 1);
    assert(0 == strcmp(mainListArr[0]->string, "and"));
    assert(0 == strcmp(mainListArr[6]->string, "welcome"));
    printf("Sorted by count:\n");
    ll_sortarray(mainListArr, mainListCount, "count");
    ll_printarray(mainListArr, mainListCount, 1);
    assert(0 == strcmp(mainListArr[0]->string, "hello"));
    assert(0 == strcmp(mainListArr[6]->string, "there"));
    assert(4 == mainListArr[0]->count);
    assert(1 == mainListArr[6]->count);

    char *newLineTestText = "asdf,";
    wordNode *newLineTestTokens = tokenize(newLineTestText, strlen(newLineTestText));
    assert(0 == strcmp(ll_lastnode(newLineTestTokens)->string, "asdf"));
    ll_print(newLineTestTokens);

    char lowercaseTestText[] = "ASDF";
    lowercase(lowercaseTestText, strlen(lowercaseTestText));
    assert(0 == strcmp(lowercaseTestText, "asdf"));

    char ignoreShortWordsTestText1[] = "a fire is upon the deep";
    wordNode *ignoreShortWordsTestTokens1 = tokenize(ignoreShortWordsTestText1, strlen(ignoreShortWordsTestText1));
    assert(4 == ll_count(ignoreShortWordsTestTokens1));
    assert(0 == strcmp("fire", ll_nodeatindex(ignoreShortWordsTestTokens1, 0)->string));

    char blankLineTestText[] = "";
    wordNode *blankLineTestTokens = tokenize(blankLineTestText, strlen(blankLineTestText));
    assert(0 == ll_count(blankLineTestTokens));

    char blankLineTest2Text[] = " \n\n  \n\r";
    wordNode *blankLineTest2Tokens = tokenize(blankLineTest2Text, strlen(blankLineTest2Text));
    assert(0 == ll_count(blankLineTest2Tokens));

    assert(5 == numberlength(12345));
    assert(1 == numberlength(5));
    assert(2 == numberlength(10));
    assert(3 == numberlength(101));
    assert(3 == numberlength(999));
    assert(10 == numberlength(1234567890));

    printf("%s\n", gettablerow("aa", "b", 2, 1, 3, 3));
    assert(0 == strcmp("|aa |b  |", gettablerow("aa", "b", 2, 1, 3, 3)));
    printf("%s\n", gettablesep(3, 3));
    assert(0 == strcmp("|---|---|", gettablesep(3, 3)));

    // misc
    char countStr[16];
    snprintf(countStr, 3, "%d", 16);
    printf("Value of countStr: \"%s\"\n", countStr);
    assert(0 == strcmp("16", countStr));

    // tokenize AND count
    char *tncText = "hello and welcome and welcome to my channel and stay";
    wordNode *tncList = tokenize_and_count(tncText, strlen(tncText));
    ll_print(tncList);
    assert(5 == ll_count(tncList));
    assert(3 == ll_getwordcount(tncList, "and"));
    assert(0 == ll_getwordcount(tncList, "asdf"));
    assert(1 == ll_getwordcount(tncList, "channel"));

    char *tncText2 = " , ";
    wordNode *tncList2 = tokenize_and_count(tncText2, strlen(tncText2));
    assert(0 == ll_count(tncList2));
    assert(0 == ll_getwordcount(tncList2, "asdf"));

    // merge lists
    char *mergeText1 = "hello and welcome to my webpage";
    char *mergeText2 = "channel channel and say hello hello and welcome to my";
    wordNode *mergeList1 = tokenize_and_count(mergeText1, strlen(mergeText1));
    wordNode *mergeList2 = tokenize_and_count(mergeText2, strlen(mergeText2));

    printf("mergeList1:\n");
    ll_print(mergeList1);
    printf("mergeList2:\n");
    ll_print(mergeList2);

    ll_mergelists(&mergeList1, &mergeList2);
    assert(3 == ll_getwordcount(mergeList1, "hello"));
    assert(0 == ll_getwordcount(mergeList1, "asdf"));
    assert(2 == ll_getwordcount(mergeList1, "channel"));
    assert(2 == ll_getwordcount(mergeList1, "welcome"));
    assert(6 == ll_count(mergeList1));
    printf("Merged lists:\n");
    ll_print(mergeList1);

    // blank list (in dest)
    char *mergeBlankText1 = "";
    char *mergeBlankText2 = "channel and say hello hello and welcome to my";
    wordNode *mergeBlankList1 = tokenize_and_count(mergeBlankText1, strlen(mergeBlankText1));
    wordNode *mergeBlankList2 = tokenize_and_count(mergeBlankText2, strlen(mergeBlankText2));
    printf("mergeBlankList1:\n");
    ll_print(mergeBlankList1);
    printf("mergeBlankList2:\n");
    ll_print(mergeBlankList2);

    ll_mergelists(&mergeBlankList1, &mergeBlankList2);
    assert(2 == ll_getwordcount(mergeBlankList1, "hello"));
    assert(0 == ll_getwordcount(mergeBlankList1, "asdf"));
    assert(1 == ll_getwordcount(mergeBlankList1, "channel"));
    assert(1 == ll_getwordcount(mergeBlankList1, "welcome"));
    assert(5 == ll_count(mergeBlankList1));
    printf("Merged lists:\n");
    ll_print(mergeBlankList1);

    // blank list (in src)
    char *mergeBlankSrcText1 = "channel and say hello hello and welcome to my";
    char *mergeBlankSrcText2 = "";
    wordNode *mergeBlankSrcList1 = tokenize_and_count(mergeBlankSrcText1, strlen(mergeBlankSrcText1));
    wordNode *mergeBlankSrcList2 = tokenize_and_count(mergeBlankSrcText2, strlen(mergeBlankSrcText2));
    printf("mergeBlankSrcList1:\n");
    ll_print(mergeBlankSrcList1);
    printf("mergeBlankSrcList2:\n");
    ll_print(mergeBlankSrcList2);

    ll_mergelists(&mergeBlankSrcList1, &mergeBlankSrcList2);
    assert(2 == ll_getwordcount(mergeBlankSrcList1, "hello"));
    assert(0 == ll_getwordcount(mergeBlankSrcList1, "asdf"));
    assert(1 == ll_getwordcount(mergeBlankSrcList1, "channel"));
    assert(1 == ll_getwordcount(mergeBlankSrcList1, "welcome"));
    assert(5 == ll_count(mergeBlankSrcList1));
    printf("Merged lists:\n");
    ll_print(mergeBlankSrcList1);

    FILE *fp;
    size_t len = 0;
    char *textLines[99999];
    for (int i = 0; i < 99999; i++)
    {
        textLines[i] = NULL;
    }
    fp = fopen("testfile1", "r");
    if (fp == NULL)
    {
        exit(EXIT_FAILURE);
    }

    // read in lines
    size_t lineCounter = 0;
    while (true)
    {
        len = getline(&textLines[lineCounter], &len, fp);
        if (-1 == len)
        {
            break;
        }
        lowercase(textLines[lineCounter], len);
        lineCounter++;
    }

    // create threads
    int numThreads = 2;
    wordNode *results[numThreads];
    for (int i = 0; i < numThreads; i++)
    {
        results[i] = malloc(sizeof(wordNode));
        results[i]->count = 0;
        results[i]->next = NULL;
        results[i]->string = NULL;
    }
    pthread_t threads[numThreads];
    size_t linesPerThread = ceil((float)lineCounter / (float)numThreads);
    printf("lineCounter/numThreads/linesPerThread: %ld/%ld/%ld\n", lineCounter, numThreads, linesPerThread);

    for (int i = 0; i < numThreads; i++)
    {
        jobSpec *js = malloc(sizeof(jobSpec));
        js->lines = textLines;
        js->lines_len = lineCounter;
        js->max = linesPerThread;
        js->results = results;
        js->results_len = numThreads;
        js->start = i * linesPerThread;
        js->thread_id = i;
        if (pthread_create(&(threads[i]), NULL, workerThread, js) != 0)
        {
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < numThreads; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < numThreads; i++)
    {
        printf("results[%d]:\n", i);
        ll_print(results[i]);
    }
}
