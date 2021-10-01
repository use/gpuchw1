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

    printf("count: %d\n", ll_count(firstWord));
    assert(3 == ll_count(firstWord));

    char *line = "it was a bright cold day in april";
    wordNode *tized = tokenize(line, strlen(line));
    ll_print(tized);
}
