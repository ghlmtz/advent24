#include "advent.h"

struct machine
{
    int ax, ay;
    int bx, by;
    int px, py;
} machine;

static long part1 = 0;

static void solve_machine()
{
    int ax = machine.ax;
    int ay = machine.ay;
    int bx = machine.bx;
    int by = machine.by;
    int px = machine.px;
    int py = machine.py;

    int rx = px/ax;
    int ry = py/ay;

    for (int i = (rx > ry ? rx : ry); i >= 0; i--)
    {
        for (int j = 0; 1; j++)
        {
            if (i * ax + j * bx == px && i * ay + j * by == py)
            {
                part1 += i * 3 + j;
                return;
            }
            if (i * ax + j * bx > px || i * ay + j * by > py)
            {
                break;
            }

        }
    }
}

static void parse_line(char *line)
{
    static int line_num = 0;
    int *nums;
    int n = scan_ints(line, &nums);
    switch(line_num % 4)
    {
        case 0:
            machine.ax = nums[0];
            machine.ay = nums[1];
            break;
        case 1:
            machine.bx = nums[0];
            machine.by = nums[1];
            break;
        case 2:
            machine.px = nums[0];
            machine.py = nums[1];
            solve_machine();
            break;
        default:
            line_num = -1;
    }
    line_num++;
    free(nums);
}

int day13()
{
    READ_INPUT("input");
    for_each_line(parse_line);
    printf("%ld\n", part1);
    return 0;
}