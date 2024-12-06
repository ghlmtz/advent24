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

static HashMap *walls;
static HashMap *tried;
static XY_POS start;
static int max_x = 0;
static int max_y = 0;
static int **paths;

static void parse_line(char *line)
{
    max_x = strlen(line);
    for(size_t x = 0; x < strlen(line); x++) 
    {
        if (line[x] == '#')
        {
            XY_POS *pos = malloc(sizeof(XY_POS));
            pos->x = x;
            pos->y = max_y;
            hash_add(walls, pos);
        }
        else if (line[x] == '^')
        {
            start.x = x;
            start.y = max_y;
        }
    }
    max_y++;
}

static int within_bounds(XY_POS *guard)
{
    return guard->x >= 0 && guard->x < max_x && guard->y >= 0 && guard->y < max_y;
}

static int test_loop(HashMap *hash_dir, XY_POS_DIR *pos, XY_POS_DIR *s)
{
    XY_POS_DIR guard_dir = {.x=s->x, .y=s->y, .dir=s->dir};
    XY_POS *guard = (XY_POS *)&guard_dir;

    while(within_bounds(guard))
    {
        XY_POS_DIR *w = malloc(sizeof(XY_POS_DIR));
        w->x = guard_dir.x;
        w->y = guard_dir.y;
        w->dir = guard_dir.dir;
        hash_add(hash_dir, w);

        if ((pos->y == guard->y && (guard_dir.dir == 1 || guard_dir.dir == 3))
         || (pos->x == guard->x && (guard_dir.dir == 0 || guard_dir.dir == 2)))  
        {
            while (!(xy_pos_eq(pos, guard) || hash_exists(walls, guard)))
            {
                xy_pos_add(guard, xy_dirs + guard_dir.dir);
                if (!within_bounds(guard))
                {
                    return 0;
                }
            }
        }
        else
        {
            int i = 0;
            int dist = paths[guard_dir.dir][max_y * guard->x + guard->y];
            int *path = &(paths[guard_dir.dir][max_y * guard->x + guard->y]);
            if (dist < 0)
            {
                return 0;
            }
            if (dist > 0)
            {
                guard->x += xy_dirs[guard_dir.dir].x * dist;
                guard->y += xy_dirs[guard_dir.dir].y * dist;
            }
            else
            {
                i = 0;
                while (!hash_exists(walls, guard))
                {
                    xy_pos_add(guard, xy_dirs + guard_dir.dir);
                    i++;
                    if (!within_bounds(guard))
                    {
                        *path = -1;
                        return 0;
                    }
                }  
                *path = i;
            }
        }
        
        xy_pos_add(guard, xy_dirs + ((guard_dir.dir + 2) % 4));
        guard_dir.dir = (guard_dir.dir + 1) % 4;

        if (hash_exists(hash_dir, guard))
        {
            return 1;
        }
    }
    return 0;
}

static void guard_walk(HashMap *hash_map)
{
    int part1 = 1;
    int part2 = 0;
    XY_POS_DIR guard = {.x=start.x, .y=start.y, .dir=0};
    XY_POS *pos = (XY_POS *)&guard;
    XY_POS_DIR prev = {.x=start.x, .y=start.y, .dir=0};
    HashMap *hash_dir = hash_init(xy_pos_hash, xy_pos_dir_eq, free);
    while(guard.x >= 0 && guard.x < max_x && guard.y >= 0 && guard.y < max_y)
    {
        XY_POS_DIR *w = malloc(sizeof(XY_POS_DIR));
        XY_POS *z = (XY_POS *)w;
        z->x = guard.x;
        z->y = guard.y;
        w->dir = guard.dir;

        hash_add(hash_map, z);

        xy_pos_add(pos, xy_dirs + guard.dir);
        if (hash_exists(walls, &guard))
        {
            xy_pos_add(pos, xy_dirs + ((guard.dir + 2) % 4));
            guard.dir = (guard.dir + 1) % 4;
        }
        if (!hash_exists(tried, z))
        {
            hash_add(tried, z);
            part2 += test_loop(hash_dir, w, &prev);
            hash_flush(hash_dir);
        }
        else
        {
            free(w);
        }
        memcpy(&prev, &guard, sizeof(XY_POS_DIR));
    }
    hash_iterate(hash_map);
    while(hash_iterate(NULL) != NULL)
        part1++;
    printf("%d\n", part1);
    printf("%d\n", part2);
    free(hash_dir);
}

int day6()
{
    HashMap *hash_map = hash_init(xy_pos_hash, xy_pos_eq, hash_dummy_free);
    tried = hash_init(xy_pos_hash, xy_pos_eq, free);
    walls = hash_init(xy_pos_hash, xy_pos_eq, free);
    READ_INPUT("input");
    for_each_line(parse_line);

    paths = malloc(sizeof(int *) * 4);
    for (int i = 0; i < 4; i++)
    {
        paths[i] = calloc(max_x * max_y, sizeof(int));
    }

    guard_walk(hash_map);

    hash_free(hash_map);
    hash_free(tried);
    hash_free(walls);
    for (int i = 0; i < 4; i++)
    {
        free(paths[i]);
    }
    free(paths);

    return 0;
}