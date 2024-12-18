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
    int dir;
    struct position *next;
};

typedef struct bfs
{
    XY_POS end;
    struct position *search;
} BFS;

static void add_to_search(BFS *bfs, int x, int y, int t, int dir)
{
    struct position *prev = bfs->search;
    struct position *this = bfs->search;
    struct position *pos = malloc(sizeof(struct position));

    pos->xy.x = x;
    pos->xy.y = y;
    pos->time = t;
    pos->dir = dir;

    if (this == NULL)
    {
        bfs->search = pos;
        pos->next = NULL;
        return;
    }

    while (this != NULL && this-> time <= pos->time)
    {
        if (this->xy.x == pos->xy.x && this->xy.y == pos->xy.y && this->dir == pos->dir)
        {
            // Duplicate, bail out
            free(pos);
            return;
        }
        prev = this;
        this = this->next;
    }

    if (this == prev)
    {
        bfs->search = pos;
    }
    else
    {
        prev->next = pos;
    }
    pos->next = this;
}

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
    BFS *bfs = malloc(sizeof(BFS));

    bfs->search = malloc(sizeof(struct position));
    bfs->search->xy.x = start_pos.x;
    bfs->search->xy.y = start_pos.y;
    bfs->search->time = 0;
    bfs->search->dir = 1;
    bfs->search->next = NULL;
    bfs->end.x = end_pos.x;
    bfs->end.y = end_pos.y;

    while (bfs->search != NULL)
    {
        struct position *top = bfs->search;
        int x = top->xy.x;
        int y = top->xy.y;
        int t = top->time + 1;
        int dir = top->dir;
        int cur = get_grid_xy(grid, top->xy);

        if (manhattan(&top->xy, &bfs->end) == 0)
        {
            int ret = top->time;
            clean_up(top);
            free(bfs);
            return ret;
        }

        if (!occupied(grid, x + xy_dirs[dir].x, y + xy_dirs[dir].y))
            add_to_search(bfs, x + xy_dirs[dir].x, y + xy_dirs[dir].y, t, dir);

        dir = modulo(dir + 1, 4);
        if (!occupied(grid, x + xy_dirs[dir].x, y + xy_dirs[dir].y))
            add_to_search(bfs, x + xy_dirs[dir].x, y + xy_dirs[dir].y, t, dir);
        
        dir = modulo(dir + 2, 4);
        if (!occupied(grid, x + xy_dirs[dir].x, y + xy_dirs[dir].y))
            add_to_search(bfs, x + xy_dirs[dir].x, y + xy_dirs[dir].y, t, dir);
        bfs->search = bfs->search->next;

        if (cur == '.')
            set_grid(grid, y, x, '#');

        free(top);
    }
    free(bfs);
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