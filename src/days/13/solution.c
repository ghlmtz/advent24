#include "advent.h"

struct machine
{
    int ax, ay;
    int bx, by;
    int px, py;
} machine;

static long part1 = 0;
static long part2 = 0;

static void cramers_rule()
{
    int ax = machine.ax;
    int ay = machine.ay;
    int bx = machine.bx;
    int by = machine.by;
    long px = machine.px;
    long py = machine.py;

    long det = ax*by - ay*bx;
    long x = px*by - py*bx;
    long y = ax*py - ay*px;
    if (x % det == 0 && y % det == 0)
        part1 += 3 * (x / det) + (y / det);

    px += 10000000000000;
    py += 10000000000000;
    x = px*by - py*bx;
    y = ax*py - ay*px;
    if (x % det == 0 && y % det == 0)
        part2 += 3 * (x / det) + (y / det);
}

/* Original part1 solver, left for posterity */
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
    scan_ints(line, &nums);
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
            (void)solve_machine;
            cramers_rule();
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
    printf("%ld\n", part2);
    return 0;
}