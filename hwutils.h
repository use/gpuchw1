#include <ctype.h>
#include <stdbool.h>

typedef struct wordNode
{
    char *string;
    struct wordNode *next;
    size_t count;
} wordNode;

wordNode *tokenize(char *line, size_t len);
char *implode(char **words, size_t numwords);
void lowercase(char *chars, size_t len);
void ll_print(wordNode *word);
bool isletter(char c);
char *ll_implode(wordNode *word, char sep);
size_t ll_count(wordNode *word);
wordNode *ll_lastnode(wordNode *word);
wordNode *ll_nodeatindex(wordNode *word, size_t n);
size_t ll_countword(wordNode **list, char *word);
void ll_deletelast(wordNode *word);
void ll_append(wordNode *word, char *str);
