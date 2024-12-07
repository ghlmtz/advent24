#include "advent.h"

static long part1 = 0;
static long part2 = 0;

static long recurse(long *nums, int n, long accum, int idx, int p2)
{
    if (accum > nums[0])
        return 0;
    if (idx == n)
        return accum == nums[0];
    if (p2)
    {
        /* Do a concat */
        long mul = 10;
        while (mul <= nums[idx])
            mul *= 10;
        if(recurse(nums, n, accum * mul + nums[idx], idx+1, p2))
            return 1;
    }
    if(recurse(nums, n, accum + nums[idx], idx+1, p2))
        return 1;
    return recurse(nums, n, accum * nums[idx], idx+1, p2);
}

static void parse_line(char *line)
{
    long *nums;
    int n = scan_longs(line, &nums);
    if (recurse(nums, n, nums[1], 2, 0))
        part1 += nums[0];
    else if (recurse(nums, n, nums[1], 2, 1))
        part2 += nums[0];

    free(nums);
}

int day7()
{
    READ_INPUT("input");
    for_each_line(parse_line);
    printf("%ld\n", part1);
    printf("%ld\n", part1 + part2);
    return 0;
}