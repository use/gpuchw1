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

    assert(0==strcmp(tokenized->string, "it"));

    wordNode *wordat1 = ll_nodeatindex(tokenized, 1);

    printf("Word at 1: %s\n", wordat1->string);
}
