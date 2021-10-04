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
size_t ll_getwordcount(wordNode *curWord, char *word);
wordNode **ll_getarray(wordNode *curWord);
void ll_printarray(wordNode *arr[], size_t arrCount, int perLine);
void ll_sortarray(wordNode *arr[], size_t arrCount, char *type);
int comparebystring(const void *a, const void *b);
int comparebycount(const void *a, const void *b);
char *gettablerow(char *a_str, char *b_str, int a_len, int b_len, int a_max, int b_max);
int numberlength(int n);
char *gettablesep(int a_max, int b_max);
void writearray(wordNode *arr[], size_t count);
wordNode *tokenize_and_count(char *line, size_t len);
