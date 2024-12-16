#include "advent.h"

static char **grid;
static int px, py;
static int line_num = 0;
static int line_len = -1;

static void parse_line(char *line)
{
    static int mode = 0;

    if (mode == 0)
    {
        if (line_len == -1) line_len = strlen(line);
        if (strlen(line) == 0)
        {
            mode = 1;
            return;
        }
        grid[line_num] = calloc(line_len, sizeof(char));
        for (int i = 0; i < line_len; i++)
        {
            if (line[i] == '#' || line[i] == 'O')
                grid[line_num][i] = line[i];
            if (line[i] == '@')
            {
                px = i;
                py = line_num;
            }
        }
        line_num++;
    }
    else
    {
        int dx, dy;
        for (int i = 0; i < strlen(line); i++)
        {
            dx = 0;
            dy = 0;
            switch(line[i])
            {
                case '^':
                    dy = -1;
                    break;
                case 'v':
                    dy = 1;
                    break;
                case '>':
                    dx = 1;
                    break;
                case '<':
                    dx = -1;
                    break;
            }
            int tmp_x = px;
            int tmp_y = py;
            do
            {
                tmp_x += dx;
                tmp_y += dy;
            } while (grid[tmp_y][tmp_x] == 'O');
            if (grid[tmp_y][tmp_x] == 0)
            {
                grid[tmp_y][tmp_x] = 'O';
                px += dx;
                py += dy;
                grid[py][px] = 0;
            }
        }
    }
}

int day15()
{
    READ_INPUT("input");
    grid = malloc(sizeof(char *) * (count_to_blank(1) - 1));
    for_each_line(parse_line);
    long gps = 0;
    for (int j = 0; j < line_num; j++)
    {
        for (int i = 0; i < line_len; i++)
        {
            if (grid[j][i] == 'O')
            {
                gps += 100 * j + i;
            }
        }
    }
    printf("%ld\n", gps);
    for (int j = 0; j < line_num; j++)
    {
        free(grid[j]);
    }
    free(grid);
    return 0;
}