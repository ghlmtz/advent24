#include "advent.h"
#include "grid.h"
#include "xy_pos.h"

#define GRID_SIZE 70
#define READ_IN 1024

static GRID *grid;
static XY_POS start_pos, end_pos;
extern const XY_POS xy_dirs[];
static int run_bfs();
static char part2[10] = {0};

static void solution(char *line)
{
    static int wall_num = 0;
    int *nums;
    if (part2[0] != 0)
        return;
    scan_ints(line, &nums);
    set_grid(grid, nums[1], nums[0], '#');
    free(nums);
    GRID *copy = grid_copy(grid);
    int result = run_bfs(copy);
    free_grid(copy);
    if (wall_num == READ_IN)
        printf("%d\n", result);
    if (result == -1)
        strcpy(part2, line);
    wall_num++;
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

int day18()
{
    READ_INPUT("input");

    grid = init_grid(GRID_SIZE + 1, GRID_SIZE + 1, '#');
    for (int i = 0; i < grid->rows; i++)
        for (int j = 0; j < grid->cols; j++)
            set_grid(grid, i, j, '.');
    start_pos = (XY_POS){.x = 0, .y = 0};
    end_pos = (XY_POS){.x = GRID_SIZE, .y = GRID_SIZE};

    for_each_line(solution);

    printf("%s\n", part2);

    free_grid(grid);

    return 0;
}