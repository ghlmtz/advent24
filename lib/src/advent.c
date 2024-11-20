#include "advent.h"

static char *buffer;

/* Reads the input into buffer. Max filesize is 2^16 - 1 bytes. */
char *read_input(const char *filename)
{
    FILE *input = fopen(filename, "r");
    size_t n;
    buffer = (char *)malloc(65536U);
    if (buffer == NULL)
    {
        printf("Memory allocation failed");
        return NULL;
    }
    n = fread(buffer, 1, 65536U, input);
    buffer[n] = '\0';
    fclose(input);
    return buffer;
}

/* Returns how many lines until a blank line is reached (or just EOF)
 * Includes blank line */
int count_to_blank(int eof)
{
    char *line = buffer;
    char *new_line = line;
    int i = 0;

    while (1)
    {
        i++;
        new_line = strstr(line, "\n");
        if (new_line == NULL ||(!eof && (new_line - line) < 3))
            break;
        new_line++;
        line = new_line;
    }

    return i;
}

void add_to_lines(char **lines, const char *line)
{
    static int line_num = 0;

    lines[line_num] = malloc(strlen(line) + 1);
    memcpy(lines[line_num++], line, strlen(line) + 1);
}

/* Runs a function on each line of the buffer. */
void for_each_line(void (func) (char *))
{
    char *line = buffer;
    char *new_line;

    while(1)
    {
        new_line = strstr(line, "\n");
        if (new_line == NULL)
            break;
    
        *new_line++ = '\0';
        func(line);

        line = new_line;
    }

    free(buffer);
}

static int compare_long_asc(const void *p, const void *q)
{
    long x = *(const long *)p;
    long y = *(const long *)q;

    if (x < y)
        return -1;
    else if (x > y)
        return 1;

    return 0;
}

static int compare_long_desc(const void *p, const void *q)
{
    long x = *(const long *)p;
    long y = *(const long *)q;

    if (x < y)
        return 1;
    else if (x > y)
        return -1;

    return 0;
}

void *bsearch_long(const long *key, const long *base, size_t nmemb)
{
    return bsearch(key, base, nmemb, sizeof(long), compare_long_asc);
}

/* Sorts array of long ints in descending order */
void sort_desc(long *longs, size_t n_longs)
{
    qsort(longs, n_longs, sizeof(long), compare_long_desc);
}

/* Sorts array of long ints in ascending order */
void sort_asc(long *longs, size_t n_longs)
{
    qsort(longs, n_longs, sizeof(long), compare_long_asc);
}

static int _strcmp(const void *p, const void *q) { return strcmp(*(const char **)p, *(const char **)q); }

size_t search_strings(char **key, char **lines, size_t rows)
{
    char **result = bsearch(key, lines, rows, sizeof(char *), _strcmp);

    if (result == NULL)
        return 0;
    else
        return (size_t)(result - lines);
}

void sort_strings(char **lines, size_t rows)
{
    qsort(lines, rows, sizeof(char *), _strcmp);
}

static int compare_char(const void *p, const void *q)
{
    char x = *(const char *)p;
    char y = *(const char *)q;

    if (x < y)
        return -1;
    else if (x > y)
        return 1;

    return 0;
}

int count_unique(const char *s)
{
    char comp;
    int ret = 1;
    int len = strlen(s);
    char *temp = malloc(len + 1);
    strcpy(temp, s);

    qsort(temp, len, sizeof(char), compare_char);

    comp = temp[len - 1];
    while (--len) {
        if (temp[len - 1] != comp)
            ret++;
        comp = temp[len - 1];
    }

    free(temp);

    return ret;
}

size_t char_index(const char *s, char ch)
{
    char *e = strchr(s, ch);
    if (e == NULL)
        return (size_t)strlen(s);
    return (size_t)(e - s);
}

size_t char_count(const char *s, char ch)
{
    size_t len = strlen(s);
    size_t count = 0;
    size_t index = char_index(s, ch);
    while(index != len) {
        count++;
        len -= index + 1;
        s += index + 1;
        index = char_index(s, ch);
    }

    return count;
}

#define SCAN_INTS_MAX 256

/* Pulls out all the integers and returns how many it found. 
 * NOTE: ignores minus sign! */
int scan_ints(char *line, int **nums)
{
    int i;

    *nums = malloc(sizeof(int) * SCAN_INTS_MAX);

    for (i = 0; i < SCAN_INTS_MAX; i++)
    {
        while (!isdigit(*line) && *line != '\0')
            line++;
        if (*line == '\0')
            break;
        sscanf(line, "%d", *nums + i);
        while (isdigit(*line))
            line++;
    }

    if (i == SCAN_INTS_MAX) {
        printf("REACHED MAXIMUM!\n");
        exit(1);
    }

    return i;
}

int scan_ints_neg(char *line, int **nums)
{
    int i;

    *nums = malloc(sizeof(int) * SCAN_INTS_MAX);

    for (i = 0; i < SCAN_INTS_MAX; i++)
    {
        while (!isdigit(*line) && *line != '-' && *line != '\0')
            line++;
        if (*line == '\0')
            break;
        sscanf(line, "%d", *nums + i);
        if (*line == '-') 
            line++;
        while (isdigit(*line))
            line++;
    }

    if (i == SCAN_INTS_MAX)
    {
        printf("REACHED MAXIMUM!\n");
        exit(1);
    }

    return i;
}

/* Pulls out all the integers and returns how many it found.
 * NOTE: ignores minus sign! */
unsigned long scan_longs(char *line, unsigned long **nums)
{
    unsigned long i;

    *nums = malloc(sizeof(unsigned long) * SCAN_INTS_MAX);

    for (i = 0; i < SCAN_INTS_MAX; i++)
    {
        while (!isdigit(*line) && *line != '\0')
            line++;
        if (*line == '\0')
            break;
        sscanf(line, "%lu", *nums + i);
        while (isdigit(*line))
            line++;
    }

    if (i == SCAN_INTS_MAX)
    {
        printf("REACHED MAXIMUM!\n");
        exit(1);
    }

    return i;
}

/* Grid stuff */

int manhattan(XY_POS *z, XY_POS *w)
{
    return abs(z->x - w->x) + abs(z->y - w->y);
}

int xy_pos_eq(void *p, void *q)
{
    XY_POS *z = (XY_POS *)p;
    XY_POS *w = (XY_POS *)q;
    return z->x == w->x && z->y == w->y;
}