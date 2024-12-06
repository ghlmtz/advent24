#include "advent.h"
#include "hashmap.h"
#include "realloc_arr.h"

ReallocArr *walls;
XY_POS guard;
const XY_POS dirs[4] = {{.x=0, .y=-1}, {.x=1, .y=0}, {.x=0, .y=1}, {.x=-1, .y=0} };
int dir = 0;
static int max_x = 0;
static int max_y = 0;

static int is_wall(XY_POS *xy)
{
    XY_POS *xy_walls = (XY_POS *)walls->elements;
    for (size_t i = 0; i < walls->length; i++)
    {
        if (xy_pos_eq(xy_walls + i, xy))
            return 1;
    }
    return 0;
}

static void parse_line(char *line)
{
    XY_POS pos;
    max_x = strlen(line);
    for(size_t x = 0; x < strlen(line); x++) 
    {
        if (line[x] == '#')
        {
            pos.x = x;
            pos.y = max_y;
            realloc_arr_add(walls, &pos);
        }
        else if (line[x] == '^')
        {
            guard.x = x;
            guard.y = max_y;
        }
    }
    max_y++;
}

void guard_walk(HashMap *hash_map)
{
    while(guard.x >= 0 && guard.x < max_x && guard.y >= 0 && guard.y < max_y)
    {
        XY_POS *z = malloc(sizeof(XY_POS));
        z->x = guard.x;
        z->y = guard.y;
        if(!hash_exists(hash_map, z))
            hash_add(hash_map, z);
        else
            free(z);
        xy_pos_add(&guard, dirs + dir);
        if (is_wall(&guard))
        {
            xy_pos_add(&guard, dirs + ((dir + 2) % 4));
            dir = (dir + 1) % 4;
        }
    }
    int count = 1;
    hash_iterate(hash_map);
    while(hash_iterate(NULL) != NULL)
        count++;
    printf("%d\n", count);
}

int day6()
{
    HashMap *hash_map = hash_init(xy_pos_hash, xy_pos_eq, free);
    READ_INPUT("input");
    walls = realloc_arr_alloc(sizeof(XY_POS));
    for_each_line(parse_line);
    guard_walk(hash_map);

    hash_free(hash_map);
    realloc_arr_free(walls);
    return 0;
}