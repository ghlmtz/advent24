#include "advent.h"
#include "grid.h"
#include "hashmap.h"
#include "xy_pos.h"

extern XY_POS xy_dirs[];
static GRID *grid;

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
        set_grid(grid, max_y, x, line[x]);
        if (line[x] == '^')
        {
            start.x = x;
            start.y = max_y;
        }
    }
    max_y++;
}

static int test_loop(HashMap *hash_dir, XY_POS_DIR *pos, XY_POS_DIR *s)
{
    XY_POS_DIR guard_dir = {.x=s->x, .y=s->y, .dir=s->dir};
    XY_POS *guard = (XY_POS *)&guard_dir;
    set_grid_xy(grid, *pos, '#');

    while(get_grid_xy(grid, *guard) != '@')
    {
        hash_add(hash_dir, &guard_dir);

        if ((pos->y == guard->y && (guard_dir.dir == 1 || guard_dir.dir == 3))
         || (pos->x == guard->x && (guard_dir.dir == 0 || guard_dir.dir == 2)))  
        {
            while (get_grid_xy(grid, *guard) != '#')
            {
                xy_pos_add(guard, xy_dirs + guard_dir.dir);
                if (get_grid_xy(grid, *guard) == '@')
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
                while (get_grid_xy(grid, *guard) != '#')
                {
                    xy_pos_add(guard, xy_dirs + guard_dir.dir);
                    i++;
                    if (get_grid_xy(grid, *guard) == '@')
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
    int part2 = 0;
    XY_POS_DIR guard = {.x=start.x, .y=start.y, .dir=0};
    XY_POS *pos = (XY_POS *)&guard;
    XY_POS_DIR prev = {.x=start.x, .y=start.y, .dir=0};
    HashMap *hash_dir = hash_init(xy_pos_hash, xy_pos_dir_eq, sizeof(XY_POS_DIR));
    while(get_grid_xy(grid, guard) != '@') 
    {
        hash_add(hash_map, pos);

        xy_pos_add(pos, xy_dirs + guard.dir);
        if (get_grid_xy(grid, guard) == '#')
        {
            xy_pos_add(pos, xy_dirs + ((guard.dir + 2) % 4));
            guard.dir = (guard.dir + 1) % 4;
        }
        if (hash_add(tried, pos) == NULL)
        {
            part2 += test_loop(hash_dir, &guard, &prev);
            set_grid_xy(grid, guard, '.');
            hash_flush(hash_dir);
        }

        memcpy(&prev, &guard, sizeof(XY_POS_DIR));
    }
    printf("%ld\n", hash_map->count);
    printf("%d\n", part2);
    hash_free(hash_dir);
}

int day6()
{
    HashMap *hash_map = hash_init(xy_pos_hash, xy_pos_eq, sizeof(XY_POS));
    tried = hash_init(xy_pos_hash, xy_pos_eq, sizeof(XY_POS));
    char *buffer = read_input("input");
    grid = init_grid_buffer(buffer, '@');
    for_each_line(parse_line);

    paths = malloc(sizeof(int *) * 4);
    for (int i = 0; i < 4; i++)
    {
        paths[i] = calloc(max_x * max_y, sizeof(int));
    }

    guard_walk(hash_map);

    hash_free(hash_map);
    hash_free(tried);
    for (int i = 0; i < 4; i++)
    {
        free(paths[i]);
    }
    free(paths);
    free_grid(grid);

    return 0;
}