#include "advent.h"

#define GRID_X 101
#define GRID_Y 103

int quadrants[4] = { 0 };

static void parse_line(char *line)
{
    int *nums;
    scan_ints_neg(line, &nums);
    nums[0] = (GRID_X + (nums[0] + nums[2]*100) % GRID_X) % GRID_X;
    nums[1] = (GRID_Y + (nums[1] + nums[3]*100) % GRID_Y) % GRID_Y;
    if (nums[0] < GRID_X/2 && nums[1] < GRID_Y/2)
        quadrants[0]++;
    if (nums[0] > GRID_X/2 && nums[1] < GRID_Y/2)
        quadrants[1]++;
    if (nums[0] > GRID_X/2 && nums[1] > GRID_Y/2)
        quadrants[2]++;
    if (nums[0] < GRID_X/2 && nums[1] > GRID_Y/2)
        quadrants[3]++;
    free(nums);
}

int day14()
{
    READ_INPUT("input");
    for_each_line(parse_line);
    printf("%ld\n", quadrants[0] * quadrants[1] * quadrants[2] * quadrants[3]);
    return 0;
}