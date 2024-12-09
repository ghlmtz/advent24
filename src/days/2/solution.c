#include "advent.h"

static int part1 = 0;
static int part2 = 0;

static int check_monotonic(int *nums, int n, int skip_index, int increase)
{
    for (int i = 1; i < n; i++)
    {
        if (i == skip_index)
            continue;
        if (skip_index == 0 && i == 1)
            continue;
        int diff = skip_index == i-1 
            ? nums[i] - nums[i-2] 
            : nums[i] - nums[i-1];
        if (increase && (diff <= 0 || diff > 3))
            return 0;
        if (!increase && (diff >= 0 || diff < -3))
            return 0;
    }
    return 1;
}

static void parse_line(char *line)
{
    int *nums;
    int n = scan_ints(line, &nums);
    if (check_monotonic(nums, n, -1, 1) || check_monotonic(nums, n, -1, 0))
    {
        part1++;
        part2++;
    }
    else 
    {
        for (int i = 0; i < n; i++)
        {
            if (check_monotonic(nums, n, i, 1) || check_monotonic(nums, n, i, 0))
            {
                part2++;
                break;
            }
        }
    }
    free(nums);
}

int day2()
{
    READ_INPUT("input");

    for_each_line(parse_line);

    printf("%d\n", part1);
    printf("%d\n", part2);
    return 0;
}