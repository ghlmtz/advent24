#include "advent.h"
#include "regex.h"

static char *gen_string(const char *base_string, size_t length)
{
    if (length >= 1000)
        return NULL;
    size_t base_len = strlen(base_string);
    size_t base_idx = 0;
    size_t idx = 0;
    char *string = calloc(sizeof(char), base_len * 7); /* 7 = X.{###}*/
    do
    {
        string[idx++] = *base_string++;
        base_idx++;
        string[idx++] = '.';
        string[idx++] = '{';
        int n = sprintf(string + idx, "%ld", length);
        idx += n;
        string[idx++] = '}';
    } while (base_idx < base_len - 1);
    string[idx] = *base_string;

    return string;
}

static char *gen_x(const char *s1, const char *s2, size_t length)
{
    if (length >= 1000)
        return NULL;    
    size_t base_len = strlen(s1);
    if (base_len % 2 != 1)
        return NULL;
    if (strlen(s2) != base_len)
        return NULL;
    size_t base_idx = 0;
    size_t idx = 0;
    char *string = calloc(sizeof(char), base_len * 14);
    size_t halfway = (base_len - 1) / 2;
    while (base_idx != base_len)
    {
        size_t dot_index = (halfway * 2) - 1;
        if (base_idx < halfway)
            string[idx++] = s1[base_idx];
        else
            string[idx++] = s2[base_idx];
        if (base_idx != halfway)
        {
            string[idx++] = '.';
            string[idx++] = '{';
            int n = sprintf(string + idx, "%ld", dot_index);
            idx += n;
            string[idx++] = '}';
            if (base_idx < halfway)
                string[idx++] = s2[base_idx];
            else
                string[idx++] = s1[base_idx];
        }
        if (base_idx != base_len - 1)
        {
            string[idx++] = '.';
            string[idx++] = '{';
            int n = sprintf(string + idx, "%ld", length - dot_index);
            idx += n;
            string[idx++] = '}';
        }
        base_idx++;
    }

    return string;
}

static char **gen_xs(const char *base_string, size_t length)
{
    size_t base_len = strlen(base_string);
    char *reverse = calloc(sizeof(char), base_len + 1);
    char **strings = malloc(sizeof(char *) * 4);
    size_t i;
    for (i = 0; i < base_len; i++)
    {
        reverse[base_len - i - 1] = base_string[i];
    }
    strings[0] = gen_x(base_string, reverse, length);
    strings[1] = gen_x(reverse, base_string, length);
    strings[2] = gen_x(base_string, base_string, length);
    strings[3] = gen_x(reverse, reverse, length);
    free(reverse);
    return strings;
}

/* Generate four directions of regex strings */
static char **gen_strings(const char *base_string, int length)
{
    char **strings = malloc(sizeof(char *) * 4);
    strings[0] = gen_string(base_string, 0);
    strings[1] = gen_string(base_string, length);
    strings[2] = gen_string(base_string, length+1);
    strings[3] = gen_string(base_string, length-1);
    return strings;
}

int day4()
{
    char *input = read_input("input");
    char *input_free = input;
    int count = 0, count2 = 0;
    size_t length = char_index(input, '\n');
    char **forward = gen_strings("XMAS", length);
    char **backward = gen_strings("SAMX", length);
    char **xs = gen_xs("MAS", length);

    while(*input != '\0')
    {
        for (int i = 0; i < 4; i++)
        {
            count += regex_match(forward[i], input);
            count += regex_match(backward[i], input);
            count2 += regex_match(xs[i], input);
        }
        input++;
    }
    printf("%d\n", count);
    printf("%d\n", count2);
    for (int i = 0; i < 4; i++)
    {
        free(forward[i]);
        free(backward[i]);
        free(xs[i]);
    }
    free(forward);
    free(backward);
    free(xs);
    free(input_free);
    return 0;
}