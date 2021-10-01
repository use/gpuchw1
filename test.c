#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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

    printf("%s", ll_implode(firstWord, '|'));
}
