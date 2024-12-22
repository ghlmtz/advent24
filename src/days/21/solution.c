#include "advent.h"
#include "day21.h"

static void n_recurse(const char *line, int idx, char *s, int s_idx)
{
    if (*line == '\0')
    {
        s[s_idx] = '\0';
        return;
    }
    int new_idx = *line - '0';
    if (*line == 'A')
        new_idx = 10;
    char *move = n_keypad[idx][new_idx];
    strcpy(s + s_idx, move);
    n_recurse(line + 1, new_idx, s, s_idx + strlen(move));
}

static int get_new_idx(char ch)
{
    switch (ch)
    {
    case '^':
        return 0;
    case 'v':
        return 1;
    case '<':
        return 2;
    case '>':
        return 3;
    default:
        return 4;
    }
}

long part2 = 0;
long do_moves(const long *old_counts_ptr, char *line, int depth)
{
    long sum = 0;
    long *old_counts = calloc(25, sizeof(long));
    memcpy(old_counts, old_counts_ptr, sizeof(long) * 25);
    while (depth > 0)
    {
        long *new_counts = calloc(25, sizeof(long));
        for (int idx = 0; idx < 5; idx++)
        {
            for (int new_idx = 0; new_idx < 5; new_idx++)
            {
                char *move = d_keypad[idx][new_idx];
                int move_idx = 4;
                while (*move != '\0')
                {
                    int move_new_idx = get_new_idx(*move);
                    new_counts[5 * move_idx + move_new_idx] += old_counts[5 * idx + new_idx];
                    move_idx = move_new_idx;
                    move++;
                }
            }
        }
        free(old_counts);
        old_counts = new_counts;
        depth--;
    }
    for (int i = 0; i < 25; i++)
        sum += old_counts[i] * atoi(line);
    free(old_counts);
    return sum;
}

long part1 = 0;
static void parse_line(char *line)
{
    char s[256] = {0};
    long counts[25] = {0};
    n_recurse(line, 10, s, 0);
    int idx = 4;
    int i = 0;
    while (s[i] != '\0')
    {
        int new_idx = get_new_idx(s[i]);
        counts[5 * idx + new_idx]++;
        idx = new_idx;
        i++;
    }
    part1 += do_moves(counts, line, 2);
    part2 += do_moves(counts, line, 25);
}

int day21()
{
    READ_INPUT("input");
    for_each_line(parse_line);

    printf("%ld\n", part1);
    printf("%ld\n", part2);

    return 0;
}