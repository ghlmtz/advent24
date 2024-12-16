#include "advent.h"
#include "grid.h"
#include "xy_pos.h"

GRID *grid;
XY_POS start_pos, end_pos;
extern const XY_POS xy_dirs[];

void solution(char *line)
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
    }

    row_num++;
}

int occupied(int x, int y)
{
    if (get_grid(grid, y, x) != '.' && get_grid(grid, y, x) != '+' && get_grid(grid, y, x) != '_') 
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

void add_to_search(BFS *bfs, int x, int y, int t, int dir)
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

void clean_up(struct position *top)
{
    struct position *prev;

    while (top != NULL)
    {
        prev = top;
        top = top->next;
        free(prev);
    }
}

int run_bfs()
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

        bfs->search = bfs->search->next;
        if (!occupied(x + xy_dirs[dir].x, y + xy_dirs[dir].y))
            add_to_search(bfs, x + xy_dirs[dir].x, y + xy_dirs[dir].y, t, dir);

        dir = modulo(dir + 1, 4);
        if (!occupied(x + xy_dirs[dir].x, y + xy_dirs[dir].y))
            add_to_search(bfs, x + xy_dirs[dir].x, y + xy_dirs[dir].y, t + 1000, dir);
        
        dir = modulo(dir + 2, 4);
        if (!occupied(x + xy_dirs[dir].x, y + xy_dirs[dir].y))
            add_to_search(bfs, x + xy_dirs[dir].x, y + xy_dirs[dir].y, t + 1000, dir);

        if (cur == '.')
            set_grid(grid, y, x, '+');
        else if (cur == '+')
            set_grid(grid, y, x, '_');
        else
            set_grid(grid, y, x, '#');

        free(top);
    }

    return -1;
}

int day16()
{
    int rows;
    int part1;
    char *buffer = read_input("input");

    grid = init_grid_buffer(buffer, '#');

    for_each_line(solution);

    part1 = run_bfs();

    printf("%d\n", part1);

    free_grid(grid);

    return 0;
}