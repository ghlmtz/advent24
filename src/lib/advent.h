#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int count_to_blank(int eof);
char *read_input(const char *);
void for_each_line(void(func)(char *));
void *bsearch_long(const long *key, const long *base, size_t nmemb);
void sort_desc(long *longs, size_t n_longs);
void sort_asc(long *longs, size_t n_longs);
void sort_strings(char **lines, size_t rows);
size_t search_strings(char **key, char **lines, size_t rows);
void add_to_lines(char **lines, const char *line);
int count_unique(const char *s);
size_t char_index(const char *s, char ch);
size_t char_count(const char *s, char ch);
int scan_ints(char *line, int **nums);
int scan_ints_neg(char *line, int **nums);
int scan_longs(char *line, long **nums);
static inline int modulo(int x, int N)
{
    return (x % N + N) % N;
}

#define READ_INPUT(x) if(read_input(x) == NULL) return 1
