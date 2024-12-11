#include "advent.h"
#include "dyn_arr.h"

long *nums;
size_t n_nums;

DynArr *a, *b;

static void parse_line(char *line)
{
    n_nums = scan_longs(line, &nums);
    for (size_t i = 0; i < n_nums; i++)
    {
        dyn_arr_add(a, nums + i);
    }
}

static void part1()
{
    b = dyn_arr_alloc(sizeof(long));
    for (size_t i = 0; i < a->length; i++)
    {
        long save;
        long el = *((long *)a->elements + i);
        if (el == 0) {
            save = 1;
            dyn_arr_add(b, &save);
        }
        else
        {
            long mul = 1;
            long exp = 0;
            while (mul <= el) {
                mul *= 10;
                exp++;
            }
            if (exp % 2 == 0)
            {
                int inc = 0;
                mul = 1;

                while (inc < exp / 2)
                {
                    mul *= 10;
                    inc++;
                }
                save = el / mul;
                dyn_arr_add(b, &save);
                save = el % mul;
                dyn_arr_add(b, &save);
            }
            else
            {
                save = el * 2024;
                dyn_arr_add(b, &save);
            }
        }
    }
    dyn_arr_free(a);
    a = b;
}

int day11()
{
    a = dyn_arr_alloc(sizeof(long));
    READ_INPUT("input");
    for_each_line(parse_line);
    for (int i = 0; i < 25; i++)
        part1();
    printf("%ld\n", a->length);
    dyn_arr_free(a);
    free(nums);
    return 0;
}