#include "advent.h"

static long **changes;
static long **prices;
static long part1 = 0;
long pseudorandom(long seed)
{
    seed = (seed ^ (seed * 64)) % 16777216;
    seed = (seed ^ (seed / 32)) % 16777216;
    seed = (seed ^ (seed * 2048)) % 16777216;
    return seed;
}

static void parse_line(char *line)
{
    static int buyer_num = 0;
    long secret = atol(line);
    prices[buyer_num] = malloc(sizeof(long) * 2001);
    changes[buyer_num] = malloc(sizeof(long) * 2000);
    prices[buyer_num][0] = secret % 10;
    for(int i = 0; i < 2000; i++)
    {
        secret = pseudorandom(secret);
        prices[buyer_num][i+1] = secret % 10;
        changes[buyer_num][i] = prices[buyer_num][i+1] - prices[buyer_num][i];
    }
    part1 += secret;
    buyer_num++;
}

int check_sequence(int buyer_num, int s1, int s2, int s3, int s4)
{
    long *prix = prices[buyer_num];
    long *change = changes[buyer_num];

    for (int i = 0; i < 2000 - 4; i++)
    {
        if (change[i] == s1 && change[i+1] == s2 && change[i+2] == s3 && change[i+3] == s4)
        {
            return prix[i+4];
        }
    }
    return 0;
}

int day22()
{
    READ_INPUT("input");
    int length = count_to_blank(1) - 1;
    changes = malloc(sizeof(long *) * length);
    prices = malloc(sizeof(long *) * length);
    for_each_line(parse_line);
    printf("%ld\n", part1);
    int best = 0;
    for (int i = -9; i <= 9; i++)
    {
        for (int j = -9; j <= 9; j++)
        {
            for (int k = -9; k <= 9; k++)
            {
                for (int q = -9; q <= 9; q++)
                {
                    int total = 0;
                    for (int b = 0; b < length; b++)
                    {
                        total += check_sequence(b, i, j, k, q);
                    }
                    best = total > best ? total : best;
                }
            }
        }
    }
    printf("%d\n", best);
    free(prices);
    free(changes);
    return 0;
}