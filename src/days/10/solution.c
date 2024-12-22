#include "advent.h"
#include "hashmap.h"
#include "xy_pos.h"
#include "grid.h"

static GRID *grid;

static void parse_line(char *line)
{
    static int y = 0;
    for(int i = 0; i < grid->cols; i++)
        set_grid(grid, y, i, line[i] - '0');
    y++;
}

static int search(HashMap *nines, int d, int i, int j)
{
    int ct = 0;
    if (d == 10)
    {
        XY_POS pos = {.x = i, .y = j};
        hash_add(nines, &pos);
        return 1;
    }
    if (get_grid(grid, i-1, j) == d)
        ct += search(nines, d+1, i-1, j);
    if (get_grid(grid, i , j-1) == d)
        ct += search(nines, d+1, i, j-1);
    if (get_grid(grid, i+1, j) == d)
        ct += search(nines, d+1, i+1, j);
    if (get_grid(grid, i, j+1) == d)
        ct += search(nines, d+1, i, j+1);
    return ct;
}

static void part1()
{
    int score = 0, rating = 0;
    HashMap *nines = hash_init(xy_pos_hash, xy_pos_eq, sizeof(XY_POS));
    for(int i = 0; i < grid->rows; i++)
    {
        for(int j = 0; j < grid->cols; j++)
            if (get_grid(grid, i, j) == 0) {
                rating += search(nines, 1, i, j);
                score += nines->count;
                hash_flush(nines);
            }
    }
    printf("%d\n", score);
    printf("%d\n", rating);
    hash_free(nines);
}

int day10()
{
    char *buffer = read_input("input");
    grid = init_grid_buffer(buffer, -1);
    for_each_line(parse_line);
    part1();
    free_grid(grid);
    return 0;
}