#include "advent.h"
#include "realloc_arr.h"

ReallocArr *num1;
ReallocArr *num2;

void parse_line(char *line)
{
    unsigned long *nums;
    scan_longs(line, &nums);
    realloc_arr_add(num1, (void *)nums[0]);
    realloc_arr_add(num2, (void *)nums[1]);
    free(nums);
}

int day1() 
{
    unsigned long diff = 0;
    unsigned long similarity = 0;
    num1 = realloc_arr_alloc(sizeof(long *));
    num2 = realloc_arr_alloc(sizeof(long *));
    READ_INPUT("input");
    for_each_line(parse_line);
    sort_asc(num1->elements, num1->length);
    sort_asc(num2->elements, num2->length);
    
    for (size_t i = 0; i < num1->length; i++)
    {
        diff += abs(num1->elements[i] - num2->elements[i]);
        for (size_t j = 0; j < num2->length; j++)
        {
            if (num2->elements[j] == num1->elements[i]) {
                similarity += num1->elements[i];
            }
            else if (num2->elements[j] > num1->elements[i])
                break;
        }
    }
    printf("%ld\n", diff);
    printf("%ld\n", similarity);
    realloc_arr_smallfree(num1);
    realloc_arr_smallfree(num2);

    return 0;
}