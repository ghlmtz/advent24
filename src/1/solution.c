#include "advent.h"

int part1 = 0;
int part2 = 0;

int day1() 
{
    char *buffer = read_input("input");

    unsigned long *nums;
    int count = scan_longs(buffer, &nums);

    for (int i = 0; i < count; i++) {
        for (int j = i + 1; j < count; j++) {
            if (nums[i] + nums[j] == 2020) {
                printf("%ld\n", nums[i] * nums[j]);
                goto next_loop;
            }
        }
    }

    next_loop:
    for (int i = 0; i < count; i++) {
        for (int j = i + 1; j < count; j++) {
            for (int k = j + 1; k < count; k++) {
                if (nums[i] + nums[j] + nums[k] == 2020) {
                    printf("%ld\n", nums[i] * nums[j] * nums[k]);
                    goto frees;
                }
            }
        }
    }

    frees:
    free(buffer);
    free(nums);

    return 0;
}