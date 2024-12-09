#include "advent.h"
#include "dyn_arr.h"

static DynArr *num1;
static DynArr *num2;

static void parse_line(char *line)
{
    long *nums;
    scan_longs(line, &nums);
    dyn_arr_add(num1, nums);
    dyn_arr_add(num2, nums + 1);
    free(nums);
}

int day1() 
{
    long diff = 0;
    long similarity = 0;
    num1 = dyn_arr_alloc(sizeof(long));
    num2 = dyn_arr_alloc(sizeof(long));
    READ_INPUT("input");
    for_each_line(parse_line);
    long *nums1 = (long *)num1->elements;
    long *nums2 = (long *)num2->elements;
    sort_asc(nums1, num1->length);
    sort_asc(nums2, num2->length);
    
    for (size_t i = 0; i < num1->length; i++)
    {
        diff += abs((int)nums1[i] - (int)nums2[i]);
        for (size_t j = 0; j < num2->length; j++)
        {
            if (nums2[j] == nums1[i]) {
                similarity += nums1[i];
            }
            else if (nums2[j] > nums1[i])
                break;
        }
    }
    printf("%ld\n", diff);
    printf("%ld\n", similarity);
    dyn_arr_free(num1);
    dyn_arr_free(num2);

    return 0;
}