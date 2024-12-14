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

static int find_bezout_coffs(int x, int y)
{
    int r0 = GRID_X, r1 = GRID_Y;
    int s0 = 1, s1 = 0;

    while (r1 != 0)
    {
        int q = r0 / r1;
        int tmp = r1;
        r1 = r0 - q * r1;
        r0 = tmp;
        tmp = s1;
        s1 = s0 - q * s1;
        s0 = tmp;
    }
    int bezout = (r0 - s0 * GRID_X) / GRID_Y;
    return (x * bezout * GRID_Y + y * s0 * GRID_X) % (GRID_X * GRID_Y);
}

static void safety_factors()
{
    static int best_x, best_y;
    static float var_x = 1000000, var_y = 1000000;
    for (int steps = 1; steps < 103; steps++)
    {
        int quadrants[4] = { 0 };
        float sum_x = 0.0, sum_y = 0.0;
        for (int i = 0; i < n_bots; i++)
        {
            struct bot bot = bots[i];

            bots[i].px = (GRID_X + (bot.px + bot.vx) % GRID_X) % GRID_X;
            bots[i].py = (GRID_Y + (bot.py + bot.vy) % GRID_Y) % GRID_Y;
            sum_x += bots[i].px;
            sum_y += bots[i].py;
            if (steps == 100)
            {
                if (bots[i].px < GRID_X/2 && bots[i].py < GRID_Y/2)
                    quadrants[0]++;
                if (bots[i].px > GRID_X/2 && bots[i].py < GRID_Y/2)
                    quadrants[1]++;
                if (bots[i].px > GRID_X/2 && bots[i].py > GRID_Y/2)
                    quadrants[2]++;
                if (bots[i].px < GRID_X/2 && bots[i].py > GRID_Y/2)
                    quadrants[3]++;

            }
        }
        if (steps == 100)
        {
            long factor = quadrants[0] * quadrants[1] * quadrants[2] * quadrants[3];
            printf("%ld\n", factor);
        }
        float this_var_x = 0.0, this_var_y = 0.0;
        float mean_x = sum_x/n_bots;
        float mean_y = sum_y/n_bots;
        for (int i = 0; i < n_bots; i++)
        {
            this_var_x += abs(mean_x - bots[i].px);
            this_var_y += abs(mean_y - bots[i].py);
        }
        if (this_var_x < var_x)
        {
            best_x = steps;
            var_x = this_var_x;
        }
        if (this_var_y < var_y)
        {
            best_y = steps;
            var_y = this_var_y;
        }
    }
    printf("%d\n", find_bezout_coffs(best_x, best_y));
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