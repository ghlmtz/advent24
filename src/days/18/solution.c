#include "advent.h"
#include "grid.h"
#include "xy_pos.h"

#define GRID_SIZE 70
#define READ_IN 1024

static GRID *grid;
static XY_POS start_pos, end_pos;
extern const XY_POS xy_dirs[];
static XY_POS *walls;

static GRID *build_grid(int idx)
{
    GRID *ret = grid_copy(grid);
    for(int i = 0; i <= idx; i++)
        set_grid_xy(ret, walls[i], '#');
    return ret;
}

static void parse_line(char *line)
{
    static int line_num = 0;
    int *nums;
    scan_ints(line, &nums);
    walls[line_num].x = nums[0];
    walls[line_num++].y = nums[1];
    free(nums);
}

static int occupied(GRID *grid, int x, int y)
{
    if (get_grid(grid, y, x) != '.') 
        return 1;
    return 0;
}

struct position
{
    XY_POS xy;
    int time;
    struct position *next;
};

static void clean_up(struct position *top)
{
    struct position *prev;

    while (top != NULL)
    {
        prev = top;
        top = top->next;
        free(prev);
    }
}

static int run_bfs(GRID *grid)
{
    struct position *search = malloc(sizeof(struct position));
    struct position *tail = search;
    search->xy.x = start_pos.x;
    search->xy.y = start_pos.y;
    search->time = 0;
    search->next = NULL;

    while (search != NULL)
    {
        struct position *top = search;
        int x = top->xy.x;
        int y = top->xy.y;
        int t = top->time + 1;
        int cur = get_grid_xy(grid, top->xy);

        if (occupied(grid, x, y))
        {
            search = search->next;
            free(top);
            continue;
        }

        if (manhattan(&top->xy, &end_pos) == 0)
        {
            int ret = top->time;
            clean_up(top);
            return ret;
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

        search = search->next;

        if (cur == '.')
            set_grid(grid, y, x, '#');

        free(top);
    }
    return -1;
}

static int* seen;

static int search(int l_idx, int h_idx)
{
    int mid = (h_idx - l_idx + 1) / 2 + l_idx;
    if (seen[mid] != 0)
        return mid;

    GRID *copy = build_grid(mid);
    seen[mid] = run_bfs(copy);
    free_grid(copy);

    return seen[mid] == -1 ? search(l_idx, mid) : search(mid, h_idx);
}

int day18()
{
    READ_INPUT("input");

    grid = init_grid(GRID_SIZE + 1, GRID_SIZE + 1, '#');
    for (int i = 0; i < grid->rows; i++)
        for (int j = 0; j < grid->cols; j++)
            set_grid(grid, i, j, '.');
    start_pos = (XY_POS){.x = 0, .y = 0};
    end_pos = (XY_POS){.x = GRID_SIZE, .y = GRID_SIZE};
    int lines = count_to_blank(1) - 1;
    seen = calloc(lines, sizeof(int));
    walls = malloc(sizeof(XY_POS) * lines);

    for_each_line(parse_line);

    GRID *copy = build_grid(READ_IN);
    printf("%d\n", run_bfs(copy));
    free_grid(copy);

    int result = search(READ_IN, lines - 1);
    printf("%d,%d\n", walls[result].x, walls[result].y);

    free(walls);
    free(seen);
    free_grid(grid);

    return 0;
}