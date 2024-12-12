#include "advent.h"
#include "hashmap.h"
#include "xy_pos.h"
#include "dyn_arr.h"

extern XY_POS xy_dirs[];

typedef struct
{
    int x;
    int y;
    int dir;
} Triplet;

static HashMap *seen;
static char **lines;
static int n_lines;

static void parse_line(char *line)
{
    add_to_lines(lines, line);
}

static void flood_fill(HashMap *region, DynArr *edges, int old_x, int old_y, int dir, char ch)
{
    int x, y;
    if (dir == -1)
    {
        x = old_x;
        y = old_y;
    }
    else
    {
        x = old_x + xy_dirs[dir].x;
        y = old_y + xy_dirs[dir].y;
    } 

    if (x < 0 || y < 0 || x >= strlen(lines[0]) || y >= n_lines || lines[y][x] != ch)
    {
        Triplet t = {.x = old_x, .y = old_y, .dir = dir};
        dyn_arr_add(edges, &t);
        return;
    }

    XY_POS *xy = malloc(sizeof(XY_POS));
    xy->x = x;
    xy->y = y;
    if (!hash_exists(region, xy))
    {
        XY_POS *seen_xy = malloc(sizeof(XY_POS));
        memcpy(seen_xy, xy, sizeof(XY_POS));
        hash_add(seen, seen_xy);
        hash_add(region, xy);
        flood_fill(region, edges, x, y, 0, ch);
        flood_fill(region, edges, x, y, 1, ch);
        flood_fill(region, edges, x, y, 2, ch);
        flood_fill(region, edges, x, y, 3, ch);
    }
    else
    {
        free(xy);
    }
}

static int in_arr(DynArr *edges_arr, Triplet *t)
{
    for (size_t i = 0; i < edges_arr->length; i++)
    {
        Triplet u = ((Triplet *)edges_arr->elements)[i];
        if (u.x == t->x && u.y == t->y && u.dir == t->dir)
        {
            ((Triplet *)edges_arr->elements)[i].dir = -1;
            return 1;
        }
    }
    return 0;
}

static int part2(DynArr *edges_arr)
{
    Triplet *edges = edges_arr->elements;
    int count = 0;
    for (size_t i = 0; i < edges_arr->length; i++)
    {
        Triplet t = edges[i];
        if (t.dir == -1)
            continue;
        count++;
        if (t.dir == 0 || t.dir == 2)
        {
            int x = t.x;
            t.x++;
            while (in_arr(edges_arr, &t))
            {
                t.x++;
            }
            t.x = x - 1;
            while (in_arr(edges_arr, &t))
            {
                t.x--;
            }
        }
        else
        {
            int y = t.y;
            t.y++;
            while (in_arr(edges_arr, &t))
            {
                t.y++;
            }
            t.y = y - 1;
            while (in_arr(edges_arr, &t))
            {
                t.y--;
            }
        }
        t.dir = -1;
    }
    return count;
}

static void part1()
{
   HashMap *region = hash_init(xy_pos_hash, xy_pos_eq, free);
   int total = 0, total2 = 0;
   for (int i = 0; i < n_lines; i++)
   {
        for (int j = 0; j < strlen(lines[0]); j++)
        {
            XY_POS xy = {.x = j, .y = i};
            if (!hash_exists(seen, &xy))
            {
                DynArr *edges = dyn_arr_alloc(sizeof(Triplet));
                flood_fill(region, edges, j, i, -1, lines[i][j]);
                total += hash_length(region) * edges->length;
                total2 += hash_length(region) * part2(edges);
                hash_flush(region);
                dyn_arr_free(edges);
            }
        }
   }
  printf("%d\n", total);
  printf("%d\n", total2);
  hash_free(region);
}

int day12()
{
    READ_INPUT("input");
    n_lines = count_to_blank(1) - 1;
    lines = malloc(sizeof(char *) * n_lines);
    for_each_line(parse_line);
    seen = hash_init(xy_pos_hash, xy_pos_eq, free);
    part1();
    hash_free(seen);
    for (int i = 0; i < n_lines; i++)
        free(lines[i]);
    free(lines);
    return 0;
}
