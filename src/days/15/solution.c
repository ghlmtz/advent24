#include "advent.h"
#include "grid.h"
#include "hashmap.h"
#include "xy_pos.h"

static GRID *grid;
static GRID *wide;
static int px, py;
static int px2, py2;
static HashMap *locs;

struct xy_pos_ch {
    int x;
    int y;
    char ch;
};

static int move_box(HashMap *locs, int box_x, int box_y, int dy)
{
    char ch = get_grid(wide, box_y, box_x);
    if (ch == '#')
        return 0;
    if (ch == '.')
        return 1;
    struct xy_pos_ch *xy = malloc(sizeof(struct xy_pos_ch));
    xy->x = box_x;
    xy->y = box_y;
    xy->ch = ch;
    if (hash_add(locs, xy))
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
    if (hash_add(locs, xy))
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
        } while (get_grid(wide, py2, tmp_x) == '[' || get_grid(wide, py2, tmp_x) == ']');
        if (get_grid(wide, py2, tmp_x) == '.')
        {
            while (tmp_x != px2)
            {
                set_grid(wide, py2, tmp_x, get_grid(wide, py2, tmp_x - dx));
                tmp_x -= dx;
            }
            px2 += dx;
        }
    }
    else /* Moving vertically requires some recursion */
    {
        if (get_grid(wide, py2 + dy, px2) == '.')
            py2 += dy;
        else if (move_box(locs, px2, py2 + dy, dy))
        {
            int moveme = px2 + 1;
            if (get_grid(wide, py2 + dy, px2) == ']')
                moveme = px2 - 1;
            struct xy_pos_ch *xy = hash_iterate(locs);
            do
            {
                set_grid(wide, xy->y, xy->x, '.');
                xy = hash_iterate(NULL);
            } while (xy != NULL);
            xy = hash_iterate(locs);
            do
            {
                set_grid(wide, xy->y + dy, xy->x, xy->ch);
                xy = hash_iterate(NULL);
            } while (xy != NULL);

            set_grid(wide, py2 + dy, moveme,'.');
            set_grid(wide, py2+dy, px2, '.');
            py2 += dy;
        }
        hash_flush(locs);
    }
}

static void parse_line(char *line)
{
    static int line_num = 0;
    static int mode = 0;

    if (mode == 0)
    {
        if (strlen(line) == 0)
        {
            mode = 1;
            return;
        }
        for (int i = 0; i < grid->cols; i++)
        {
            set_grid(grid, line_num, i, line[i]);
            if (line[i] == '@')
            {
                set_grid(grid, line_num, i, '.');
                px = i;
                py = line_num;
                px2 = 2*i;
                py2 = line_num;
            }
            switch(line[i])
            {
                case '#':
                    set_grid(wide, line_num, 2*i, '#');
                    set_grid(wide, line_num, 2*i+1, '#');
                    break;
                case 'O':
                    set_grid(wide, line_num, 2*i, '[');
                    set_grid(wide, line_num, 2*i+1, ']');
                    break;
                default:
                    set_grid(wide, line_num, 2*i, '.');
                    set_grid(wide, line_num, 2*i+1, '.');                
            }
        }
        line_num++;
    }
    else
    {
        int dx, dy;
        for (size_t i = 0; i < strlen(line); i++)
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
            } while (get_grid(grid, tmp_y, tmp_x) == 'O');
            if (get_grid(grid, tmp_y, tmp_x) == '.')
            {
                set_grid(grid, tmp_y, tmp_x, 'O');
                px += dx;
                py += dy;
                set_grid(grid, py, px, '.');
            }
            part2(dx, dy);
        }
    }
}

static void print_gps(GRID *g, char ch)
{
    long gps = 0;
    for (int j = 0; j < g->rows; j++)
        for (int i = 0; i < g->cols; i++)
            if (get_grid(g, j, i) == ch)
                gps += 100 * j + i;
    printf("%ld\n", gps);
    free_grid(g);
}

int day15()
{
    char *buffer = read_input("input");
    grid = init_grid(count_to_blank(0) - 1, char_index(buffer, '\n'), '#');
    wide = init_grid(grid->rows, grid->cols * 2, '#');
    locs = hash_init(xy_pos_hash, xy_pos_eq, free);
    for_each_line(parse_line);
    print_gps(grid, 'O');
    print_gps(wide, '[');
    free_grid(wide);
    hash_free(locs);
    return 0;
}