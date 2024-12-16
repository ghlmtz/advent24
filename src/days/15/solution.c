#include "advent.h"
#include "hashmap.h"
#include "xy_pos.h"

static char **grid;
static char **widegrid;
static int init_px, init_py;
static int px, py;
static int px2, py2;
static int line_num = 0;
static int line_len = -1;
static HashMap *locs;

struct xy_pos_ch {
    int x;
    int y;
    char ch;
};

static debug_print()
{
    for (int j = 0; j < line_num; j++)
    {
        for (int i = 0; i < 2*line_len; i++)
        {
            char ch = widegrid[j][i];
            if (ch == 0)
                ch = '.';
            if (i == px2 && j == py2)
                ch = '@';
            printf("%c", ch);
        }
        printf("\n");
    }
}

static int move_box(HashMap *locs, int box_x, int box_y, int dy)
{
    char ch = widegrid[box_y][box_x];
    if (ch == '#')
        return 0;
    if (ch == 0)
        return 1;
    struct xy_pos_ch *xy = malloc(sizeof(struct xy_pos_ch));
    xy->x = box_x;
    xy->y = box_y;
    xy->ch = ch;
    if (!hash_exists(locs, xy))
        hash_add(locs, xy);
    else
        free(xy);
    xy = malloc(sizeof(struct xy_pos_ch));
    if (ch == '[')
    {
        xy->x = box_x + 1;
        xy->y = box_y;
        xy->ch = ']';
    }
    else
    {
        xy->x = box_x - 1;
        xy->y = box_y;
        xy->ch = '[';
    }
    if (!hash_exists(locs, xy))
        hash_add(locs, xy);
    else
        free(xy);


    if (ch == '[' && move_box(locs, box_x + 1, box_y + dy, dy))
    {
        return move_box(locs, box_x, box_y + dy, dy);
    }
    if (ch == ']' && move_box(locs, box_x - 1, box_y + dy, dy))
        return move_box(locs, box_x, box_y + dy, dy);
    return 0;
}

static void part2(int dx, int dy)
{
    if (dx != 0) /* Moving horizontally is similar to before */
    {
        int tmp_x = px2;
        do
        {
            tmp_x += dx;
        } while (widegrid[py2][tmp_x] == '[' || widegrid[py2][tmp_x] == ']');
        if (widegrid[py2][tmp_x] == 0)
        {
            while (tmp_x != px2)
            {
                widegrid[py2][tmp_x] = widegrid[py2][tmp_x - dx];
                tmp_x -= dx;
            }
            px2 += dx;
        }
    }
    else /* Moving vertically requires some recursion */
    {
        if (widegrid[py2 + dy][px2] == 0)
            py2 += dy;
        else if (move_box(locs, px2, py2 + dy, dy))
        {
            int moveme = px2 + 1;
            if (widegrid[py2 + dy][px2] == ']')
                moveme = px2 - 1;
            struct xy_pos_ch *xy = hash_iterate(locs);
            do
            {
                widegrid[xy->y][xy->x] = 0;
                xy = hash_iterate(NULL);
            } while (xy != NULL);
            xy = hash_iterate(locs);
            do
            {
                widegrid[xy->y + dy][xy->x] = xy->ch;
                xy = hash_iterate(NULL);
            } while (xy != NULL);

            widegrid[py2 + dy][moveme] = 0;
            widegrid[py2+dy][px2] = 0;
            py2 += dy;
        }
        hash_flush(locs);
    }
    //printf(" %d %d\n", px2, py2);
    //debug_print();
}

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
        widegrid[line_num] = calloc(2 * line_len, sizeof(char));
        for (int i = 0; i < line_len; i++)
        {
            if (line[i] == '#' || line[i] == 'O')
            {
                grid[line_num][i] = line[i];
            }
            switch(grid[line_num][i])
            {
                case '#':
                    widegrid[line_num][2*i] = '#';
                    widegrid[line_num][2*i+1] = '#';
                    break;
                case 'O':
                    widegrid[line_num][2*i] = '[';
                    widegrid[line_num][2*i+1] = ']';
                    break;
            }
            if (line[i] == '@')
            {
                px = i;
                py = line_num;
                px2 = 2*i;
                py2 = line_num;
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
            part2(dx, dy);
        }
    }
}

int day15()
{
    READ_INPUT("input");
    int lines = count_to_blank(0) - 1;
    grid = malloc(sizeof(char *) * lines);
    widegrid = malloc(sizeof(char *) * lines);
    locs = hash_init(xy_pos_hash, xy_pos_eq, free);
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
    gps = 0;
    for (int j = 0; j < line_num; j++)
    {
        for (int i = 0; i < line_len * 2; i++)
        {
            if (widegrid[j][i] == '[')
            {
                gps += 100 * j + i;
            }
        }
    }
    printf("%ld\n", gps);
    for (int j = 0; j < line_num; j++)
    {
        free(widegrid[j]);
    }
    free(widegrid);
    hash_free(locs);
    return 0;
}