#include "advent.h"

static long part1 = 0;
static long part2 = 0;
static long master_sequence[19 * 19 * 19 * 19] = {0};

long pseudorandom(long seed)
{
    seed = (seed ^ (seed * 64)) % 16777216;
    seed = (seed ^ (seed / 32)) % 16777216;
    seed = (seed ^ (seed * 2048)) % 16777216;
    return seed;
}

static void parse_line(char *line)
{
    long secret = atol(line);
    int seen[19*19*19*19] = {0};
    long c0 = 0, c1 = 0, c2 = 0, c3 = 0;
    long price = secret % 10;
    for(int i = 0; i < 2000; i++)
    {
        if (i >= 4)
        {
            long idx = (c0 + 9) + (c1 + 9) * 19 \
                + (c2+9) * 19 * 19 + (c3+9) * 19 * 19 * 19;
            if (!seen[idx])
            {
                master_sequence[idx] += price;
                if (master_sequence[idx] > part2)
                    part2 = master_sequence[idx];
            }
            seen[idx]++;
        }
        secret = pseudorandom(secret);
        c0 = c1;
        c1 = c2;
        c2 = c3;
        c3 = secret % 10 - price;
        price = secret % 10;
    }
    part1 += secret;
}

int day22()
{
    READ_INPUT("input");
    for_each_line(parse_line);
    printf("%ld\n", part1);
    printf("%ld\n", part2);
    return 0;
}