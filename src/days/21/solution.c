#include "advent.h"
#include "day21.h"

#define STRING_SIZE 25600

static size_t d_recurse(const char *line, int idx, char *s, size_t s_idx, int depth)
{
    if (*line == '\0')
    {
        s[s_idx] = '\0';
        if (depth == 1)
        {
            return strlen(s);
        }
        else
        {
            char d[STRING_SIZE] = {0};
            return d_recurse(s, 4, d, 0, depth + 1);
        }
    }
    int new_idx;
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
    
    char *move = d_keypad[idx][new_idx][0];
    strcpy(s + s_idx, move);
    return d_recurse(line + 1, new_idx, s, s_idx + strlen(move), depth);
}

static size_t n_recurse(const char *line, int idx, char *s, int s_idx)
{
    if (*line == '\0')
    {
        s[s_idx] = '\0';
        char d[STRING_SIZE] = {0};
        return d_recurse(s, 4, d, 0, 0);
    }
    int new_idx = *line == 'A' ? 10 : *line - '0';

    char *move = n_keypad[idx][new_idx][0];
    strcpy(s + s_idx, move);
    return n_recurse(line + 1, new_idx, s, s_idx + strlen(move));
}

int part1 = 0;
static void parse_line(char *line)
{
    char s[STRING_SIZE] = {0};
    part1 += n_recurse(line, 10, s, 0) * atoi(line);
}

int day21()
{
    READ_INPUT("input");
    for_each_line(parse_line);

    printf("%d\n", part1);

    return 0;
}