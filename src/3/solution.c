#include "advent.h"

static long part1 = 0;
static long part2 = 0;
static int enabled = 1;

static void parse_line(char *line)
{
    char *match;
    int a, b;
    char *next_do = strstr(line, "do()");
    char *next_dont = strstr(line, "don't()");
    while((match = strstr(line, "mul(")) != NULL)
    {
        if (next_do != NULL && match > next_do)
            enabled = 1;
        if (next_dont != NULL && match > next_dont)
            enabled = 0;
        a = 0;
        b = 0;
        line = match + 4;
        while (isdigit(*line))
        {
            a = 10 * a + *line - '0';
            line++;
        }
        if (*line != ',')
            continue;
        line++;
        while (isdigit(*line))
        {
            b = 10 * b + *line - '0';
            line++;
        }
        if (*line != ')')
            continue;
        line++;
        part1 += a * b;
        if (enabled)
            part2 += a * b;
        next_do = strstr(line, "do()");
        next_dont = strstr(line, "don't()");
    }
}

int day3()
{
    READ_INPUT("input");
    for_each_line(parse_line);
    printf("%ld\n", part1);
    printf("%ld\n", part2);
    return 0;
}