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
    lines[0] = "hello and welcome to my channel";
    lines[1] = "hello and welcome to my webpage";
    lines[2] = "hey there welcome to my webpage";

    wordNode *tokenLists[numLines];
    for (size_t i = 0; i < 3; i++)
    {
        tokenLists[i] = tokenize(lines[i], strlen(lines[i]));
        printf("%s\n", ll_implode(tokenLists[i], '_'));
    }
}
