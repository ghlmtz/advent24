#include "advent.h"
#include "dyn_arr.h"
#include "grid.h"
#include "xy_pos.h"

static GRID *grid;
static GRID *distances;
static XY_POS start_pos, end_pos;
extern const XY_POS xy_dirs[];
static DynArr *spaces;

static void parse_line(char *line)
{
    static int row_num = 0;
    int i;

    for (i = 0; i < grid->cols; i++)
    {
        if (line[i] == 'S') {
            start_pos.y = row_num;
            start_pos.x = i;
            set_grid(grid, row_num, i, '.');
        }
        else if (line[i] == 'E') {
            end_pos.y = row_num;
            end_pos.x = i;
            set_grid(grid, row_num, i, '.');
        }
        else {
            set_grid(grid, row_num, i, line[i]);
        }

        if (line[i] != '#')
        {
            XY_POS xy = {.x = i, .y = row_num};
            dyn_arr_add(spaces, &xy);
        }
    }

    row_num++;
}

static int occupied(GRID *grid, int x, int y)
{
    if (get_grid(grid, y, x) != '.') 
        return 1;
    if (get_grid(distances, y, x) != -1)
        return 1;
    return 0;
}

struct position
{
    XY_POS xy;
    int time;
    struct position *next;
};

static int run_bfs(GRID *grid)
{
    struct position *search = malloc(sizeof(struct position));
    struct position *tail = search;
    search->xy.x = end_pos.x;
    search->xy.y = end_pos.y;
    search->time = 0;
    search->next = NULL;

    while (search != NULL)
    {
        struct position *top = search;
        int x = top->xy.x;
        int y = top->xy.y;
        int t = top->time + 1;

        if (occupied(grid, x, y))
        {
            search = search->next;
            free(top);
            continue;
        }

        for (int dir = 0; dir < 4; dir++)
            if (!occupied(grid, x + xy_dirs[dir].x, y + xy_dirs[dir].y))
            {
                struct position *pos = malloc(sizeof(struct position));
                pos->xy.x = x + xy_dirs[dir].x;
                pos->xy.y = y + xy_dirs[dir].y;
                pos->time = t;
                pos->next = NULL;
                tail->next = pos;
                tail = pos;
            }

        set_grid_xy(distances, top->xy, t - 1);
        search = search->next;

        free(top);
    }
    return -1;
}

static int cheat(int time_saved)
{
    int count = 0;
    for (int i = 0; i < grid->rows; i++)
    {
        for (int j = 0; j < grid->cols; j++)
        {
            if (get_grid(distances, i, j) != -1)
                continue;
            int x = get_grid(distances, i+1, j);
            int y = get_grid(distances, i-1, j);
            if (x != -1 && y != -1 && abs(y - x) - 2 >= time_saved)
                count++;

            x = get_grid(distances, i, j+1);
            y = get_grid(distances, i, j-1);
            if (x != -1 && y != -1 && abs(y - x) - 2 >= time_saved)
                count++;
        }
    }
    return count;
}

static int cheat2(int time_saved)
{
    int count = 0;
    for (size_t i = 0; i < spaces->length; i++)
    {
        XY_POS xy = DYN_ARR_GET(XY_POS, spaces, i);
        int xy_dist = get_grid_xy(distances, xy);
        for (int dy = -20; dy <= 20; dy++)
        {
            for (int dx = -(20-abs(dy)); dx <= 20-abs(dy); dx++)
            {
                int zw_dist = get_grid(distances, xy.y + dy, xy.x + dx);
                if (zw_dist != -1 && xy_dist - zw_dist - abs(dy) - abs(dx) >= time_saved)
                    count++;
            }
        }
    }
    return count;
}

int day20()
{
    char *buffer = read_input("input");
    grid = init_grid_buffer(buffer, '#');
    distances = init_grid_buffer(buffer, -1);
    spaces = dyn_arr_alloc(sizeof(XY_POS));
    for_each_line(parse_line);

    memset(distances->data, -1, sizeof(int) * distances->rows * distances->cols);

    run_bfs(grid);
    printf("%d\n", cheat(100));
    printf("%d\n", cheat2(100));
    free_grid(grid);
    free_grid(distances);
    dyn_arr_free(spaces);
    return 0;
}