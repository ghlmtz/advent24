#include "advent.h"

#define GRID_X 101
#define GRID_Y 103

static int n_bots;
struct bot {
    int px;
    int py;
    int vx;
    int vy;
} *bots;

static void safety_factors()
{
    static int safest;
    static int safest_factor = 0xFFFFFFF;
    for (int steps = 1; steps < GRID_X * GRID_Y; steps++)
    {
        int quadrants[4] = { 0 };
        for (int i = 0; i < n_bots; i++)
        {
            struct bot bot = bots[i];

            bots[i].px = (GRID_X + (bot.px + bot.vx) % GRID_X) % GRID_X;
            bots[i].py = (GRID_Y + (bot.py + bot.vy) % GRID_Y) % GRID_Y;
            if (bots[i].px < GRID_X/2 && bots[i].py < GRID_Y/2)
                quadrants[0]++;
            if (bots[i].px > GRID_X/2 && bots[i].py < GRID_Y/2)
                quadrants[1]++;
            if (bots[i].px > GRID_X/2 && bots[i].py > GRID_Y/2)
                quadrants[2]++;
            if (bots[i].px < GRID_X/2 && bots[i].py > GRID_Y/2)
                quadrants[3]++;
        }
        long factor = quadrants[0] * quadrants[1] * quadrants[2] * quadrants[3];
        if (steps == 100)
            printf("%ld\n", factor);
        if (factor < safest_factor)
        {
            safest = steps;
            safest_factor = factor;
        }
    }
    printf("%d\n", safest);
}

static void parse_line(char *line)
{
    static int bot_num = 0;
    int *nums;
    scan_ints_neg(line, &nums);
    bots[bot_num].px = nums[0];
    bots[bot_num].py = nums[1];
    bots[bot_num].vx = nums[2];
    bots[bot_num].vy = nums[3];
    free(nums);
    bot_num++;
}

int day14()
{
    READ_INPUT("input");
    n_bots = count_to_blank(1) - 1;
    bots = malloc(sizeof(struct bot) * n_bots);
    for_each_line(parse_line);
    safety_factors();

    free(bots);
    return 0;
}