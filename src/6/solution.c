#include "advent.h"
#include "hashmap.h"
#include "realloc_arr.h"
#include "xy_pos.h"

extern XY_POS xy_dirs[];

typedef struct {
    int x;
    int y;
    int dir;
} XY_POS_DIR;

int xy_pos_dir_eq(void *p, void *q)
{
    XY_POS_DIR *z = (XY_POS_DIR *)p;
    XY_POS_DIR *w = (XY_POS_DIR *)q;
    return z->x == w->x && z->y == w->y && z->dir == w->dir;
}

static ReallocArr *walls;
static HashMap *tried;
static XY_POS start;
static int dir = 0;
static int max_x = 0;
static int max_y = 0;

static int is_wall(XY_POS *xy)
{
    return NULL != bsearch(xy, walls->elements, walls->length, sizeof(XY_POS), xy_pos_comp);
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
            start.x = x;
            start.y = max_y;
        }
    }
    max_y++;
}

static int test_loop(XY_POS_DIR *pos)
{
    XY_POS_DIR guard_dir = {.x=start.x, .y=start.y, .dir=0};
    XY_POS *guard = (XY_POS *)&guard_dir;
    HashMap *hash_dir = hash_init(xy_pos_hash, xy_pos_dir_eq, free);
    while(guard->x >= 0 && guard->x < max_x && guard->y >= 0 && guard->y < max_y)
    {
        XY_POS_DIR *w = malloc(sizeof(XY_POS_DIR));
        w->x = guard_dir.x;
        w->y = guard_dir.y;
        w->dir = guard_dir.dir;
        hash_add(hash_dir, w);

        xy_pos_add(guard, xy_dirs + guard_dir.dir);
        if (xy_pos_eq(pos, guard) || is_wall(guard))
        {
            xy_pos_add(guard, xy_dirs + ((guard_dir.dir + 2) % 4));
            guard_dir.dir = (guard_dir.dir + 1) % 4;
        }
        if (hash_exists(hash_dir, guard))
        {
            hash_free(hash_dir);
            return 1;
        }
    }
    hash_free(hash_dir);
    return 0;
}

void guard_walk(HashMap *hash_map)
{
    int part2 = 0;
    XY_POS guard = {.x=start.x, .y=start.y};
    while(guard.x >= 0 && guard.x < max_x && guard.y >= 0 && guard.y < max_y)
    {
        XY_POS_DIR *w = malloc(sizeof(XY_POS_DIR));
        XY_POS *z = (XY_POS *)w;
        z->x = guard.x;
        z->y = guard.y;
        w->dir = dir;

        hash_add(hash_map, z);

        xy_pos_add(&guard, xy_dirs + dir);
        if (is_wall(&guard))
        {
            xy_pos_add(&guard, xy_dirs + ((dir + 2) % 4));
            dir = (dir + 1) % 4;
            w->dir = dir;
        }
        if (!(guard.x == start.x && guard.y == start.y && dir == 0) && !hash_exists(tried, z))
        {
            hash_add(tried, z);
            part2 += test_loop(w);
        }
        else
        {
            free(w);
        }
    }
    int count = 1;
    hash_iterate(hash_map);
    while(hash_iterate(NULL) != NULL)
        count++;
    printf("%d\n", count);
    printf("%d\n", part2);
}

int day6()
{
    HashMap *hash_map = hash_init(xy_pos_hash, xy_pos_eq, hash_dummy_free);
    tried = hash_init(xy_pos_hash, xy_pos_eq, free);
    walls = realloc_arr_alloc(sizeof(XY_POS));
    READ_INPUT("input");
    for_each_line(parse_line);
    guard_walk(hash_map);

    hash_free(hash_map);
    hash_free(tried);
    realloc_arr_free(walls);
    return 0;
}