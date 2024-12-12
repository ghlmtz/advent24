#include "advent.h"
#include "hashmap.h"
#include "xy_pos.h"

static HashMap *seen;
static char **lines;
static int n_lines;

static void parse_line(char *line)
{
    add_to_lines(lines, line);
}

static int flood_fill(HashMap *region, int x, int y, char ch)
{
    if (x < 0 || y < 0 || x >= strlen(lines[0]) || y >= n_lines)
        return 1;
    if (lines[y][x] != ch)
        return 1;
    XY_POS *xy = malloc(sizeof(XY_POS));
    xy->x = x;
    xy->y = y;
    if (!hash_exists(region, xy))
    {
        XY_POS *seen_xy = malloc(sizeof(XY_POS));
        memcpy(seen_xy, xy, sizeof(XY_POS));
        hash_add(seen, seen_xy);
        hash_add(region, xy);
        return flood_fill(region, x+1, y, ch) +
            flood_fill(region, x-1, y, ch) +
            flood_fill(region, x, y+1, ch) +
            flood_fill(region, x, y-1, ch);
    }
    else
    {
        free(xy);
        return 0;
    }
}

static void part1()
{
   HashMap *region = hash_init(xy_pos_hash, xy_pos_eq, free);
   int total = 0;
   for (int i = 0; i < n_lines; i++)
   {
        for (int j = 0; j < strlen(lines[0]); j++)
        {
            XY_POS xy = {.x = j, .y = i};
            if (!hash_exists(seen, &xy))
            {
                int count = flood_fill(region, j, i, lines[i][j]);
                total += hash_length(region) * count;
                hash_flush(region);
            }
        }
   }
  printf("%d\n", total);
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
