#include "advent.h"
#include "hashmap.h"
#include "xy_pos.h"

static XY_POS antennae[62][10];
static int n_antennae[62] = { 0 };
static int max_x = 0, max_y = 0;

static size_t get_index(char ch)
{
    if (isdigit(ch))
        return ch - '0';
    if (isupper(ch))
        return ch - 'A' + 10;
    return ch - 'a' + 36;
}

static void parse_line(char *line)
{
    int x = 0;
    while (line[x] != '\0')
    {
        if (line[x] != '.')
        {
            int idx = get_index(line[x]);
            antennae[idx][n_antennae[idx]].x = x;
            antennae[idx][n_antennae[idx]++].y = max_y;
        }
        x++;
    }
    max_x = x;
    max_y++;
}

static void solve()
{
    HashMap *seen1 = hash_init(xy_pos_hash, xy_pos_eq, free);
    HashMap *seen2 = hash_init(xy_pos_hash, xy_pos_eq, free);
    XY_POS diff;
    XY_POS scratch;
    for (int i = 0; i < 62; i++)
    {
        for (int a = 0; a < n_antennae[i]; a++)
        {
            if (n_antennae[i] > 1)
            {
                XY_POS *store = malloc(sizeof(XY_POS));
                store->x = antennae[i][a].x;
                store->y = antennae[i][a].y;
                if (!hash_exists(seen2, store))
                    hash_add(seen2, store);
                else
                    free(store);
            }
            for (int b = 0; b < n_antennae[i]; b++)
            {
                if (a == b)
                    continue;
                memcpy(&scratch, &antennae[i][b], sizeof(XY_POS));
                diff.x = antennae[i][b].x - antennae[i][a].x;
                diff.y = antennae[i][b].y - antennae[i][a].y;
                int first = 1;
                while(1)
                {
                    xy_pos_add(&scratch, &diff);
                    if (scratch.x >= 0 && scratch.x < max_x && scratch.y >= 0 && scratch.y < max_y)
                    {
                        XY_POS *store;
                        if (first)
                        {
                            store = malloc(sizeof(XY_POS));
                            memcpy(store, &scratch, sizeof(XY_POS));
                            if (!hash_exists(seen1, store))
                                hash_add(seen1, store);
                            else
                                free(store);
                        }
                        store = malloc(sizeof(XY_POS));
                        memcpy(store, &scratch, sizeof(XY_POS));
                        if (!hash_exists(seen2, store))
                            hash_add(seen2, store);
                        else
                            free(store);
                    }
                    else
                        break;
                    first = 0;
                }
            }
        }
    }
    printf("%ld\n", hash_length(seen1));
    printf("%ld\n", hash_length(seen2));
    hash_free(seen1);
    hash_free(seen2);
}

int day8()
{

    READ_INPUT("input");
    for_each_line(parse_line);
    solve();
    return 0;
}