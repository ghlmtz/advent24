#include "advent.h"
#include "hashmap.h"
#include "xy_pos.h"

static int rating = 0;
static int grid_size;
static int **grid;

static void parse_line(char *line)
{
    static int y = 0;
    grid[y] = malloc(sizeof(int) * grid_size);
    for(int i = 0; i < grid_size; i++)
        grid[y][i] = line[i] - '0';
    y++;
}

static void search(HashMap *nines, int d, int i, int j)
{
    if (d == 10)
    {
        XY_POS *pos = malloc(sizeof(XY_POS));
        pos->x = i;
        pos->y = j;
        if (!hash_exists(nines, pos))
            hash_add(nines, pos);
        else
            free(pos);
        rating++;
        return;
    }
    if (i-1 >= 0 && grid[i-1][j] == d)
        search(nines, d+1, i-1, j);
    if (j-1 >= 0 && grid[i][j-1] == d)
        search(nines, d+1, i, j-1);
    if (i+1 < grid_size && grid[i+1][j] == d)
        search(nines, d+1, i+1, j);
    if (j+1 < grid_size && grid[i][j+1] == d)
        search(nines, d+1, i, j+1);
}

static void part1()
{
    int score = 0;
    HashMap *nines = hash_init(xy_pos_hash, xy_pos_eq, free);
    for(int i = 0; i < grid_size; i++)
    {
        for(int j = 0; j < grid_size; j++)
            if (grid[i][j] == 0) {
                search(nines, 1, i, j);
                score += hash_length(nines);
                hash_flush(nines);
            }
    }
    printf("%d\n", score);
    printf("%d\n", rating);
    hash_free(nines);
}

int day10()
{
    READ_INPUT("input");
    grid_size = count_to_blank(0) - 1;
    grid = malloc(sizeof(int *) * grid_size);
    for_each_line(parse_line);
    part1();
    for (int i = 0; i < grid_size; i++)
        free(grid[i]);
    free(grid);
    return 0;
}