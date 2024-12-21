#include "advent.h"
#include "day21.h"

size_t min = 256;

static void d_recurse(const char *line, int idx, char *s, size_t s_idx, int depth)
{
    if (*line == '\0')
    {
        s[s_idx] = '\0';
        if (depth == 1)
        {
            min = strlen(s) < min ? strlen(s) : min; // !!!
        }
        else
        {
            char *d = calloc(256, sizeof(char));
            d_recurse(s, 4, d, 0, depth + 1);
            free(d);
        }
        return;
    }
    if (s_idx > min)
        return;
    int new_idx = *line - '0';
    switch(*line)
    {
        case '^':
            new_idx = 0;
            break;
        case 'v':
            new_idx = 1;
            break;
        case '<':
            new_idx = 2;
            break;
        case '>':
            new_idx = 3;
            break;
        default:
            new_idx = 4;
            break;
    }
    
    char *move = d_keypad[idx][new_idx];
    strcpy(s + s_idx, move);
    d_recurse(line + 1, new_idx, s, s_idx + strlen(move), depth);
}

static void n_recurse(const char *line, int idx, char *s, int s_idx)
{
    if (*line == '\0')
    {
        s[s_idx] = '\0';
        char *d = calloc(256, sizeof(char));
        d_recurse(s, 4, d, 0, 0);
        free(d);
        return;
    }
    int new_idx = *line - '0';
    if (*line == 'A')
        new_idx = 10;
    for (int i = 0; i < 2; i++)
    {
        char *move = n_keypad[idx][new_idx][i];
        if (move == NULL)
            continue;
        strcpy(s + s_idx, move);
        n_recurse(line + 1, new_idx, s, s_idx + strlen(move));
    }
}

int part1 = 0;
static void parse_line(char *line)
{
    min = 256;
    char *s = calloc(256, sizeof(char));
    n_recurse(line, 10, s, 0);
    part1 += min * atoi(line);
    free(s);
}

int day21()
{
    READ_INPUT("input");
    for_each_line(parse_line);

    printf("%d\n", part1);

    return 0;
}