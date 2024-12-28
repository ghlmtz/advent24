#include "advent.h"

#define ALPHA 26 * 26

static int present[ALPHA] = {0};
static int link[ALPHA][ALPHA] = {0};
static int adj[ALPHA][ALPHA] = {0};

static void parse_line(char *line)
{
    int node1 = (line[0]-'a') * 26 + (line[1]-'a');
    int node2 = (line[3]-'a') * 26 + (line[4]-'a');
    adj[node1][node2] = 1;
    adj[node2][node1] = 1;
    present[node1] = 1;
    present[node2] = 1;
    if (node1 < node2)
        link[node1][node2] = 1;
    else
        link[node2][node1] = 1;
}

int found_long = 0;

void bron_kerbosch(int *r, int *p, int *x, int n_r)
{
    int empty = 1;
    for (int i = 0; i < ALPHA; i++)
    {
        if (p[i] != 0 || x[i] != 0) {
            empty = 0;
            break;
        }
    }
    if (empty) {
        if (n_r > 12)
        {
            char print_me[100] = {0};
            int ch = 0;
            for (int i = 0; i < ALPHA; i++)
            {
                if (r[i]) {
                    sprintf(print_me + (ch++) * 3, "%c%c,", i / 26 + 'a', i % 26 + 'a');
                }
            }
            print_me[ch * 3 - 1] = '\0';
            printf("%s\n", print_me);
            found_long = 1;
        }
        return;
    }
    for (int i = 0; i < ALPHA; i++)
    {
        if (!p[i])
            continue;
        r[i] = 1;
        int *new_p = calloc(ALPHA, sizeof(int));
        int *new_x = calloc(ALPHA, sizeof(int));
        for (int n = 0; n < ALPHA; n++)
        {
            if (adj[i][n]) {
                if (p[n])
                    new_p[n] = 1;
                if (x[n])
                    new_x[n] = 1;
            } 
        }
        bron_kerbosch(r, new_p, new_x, n_r + 1);
        free(new_p);
        free(new_x);
        r[i] = 0;
        p[i] = 0;
        x[i] = 1;
        if (found_long)
            return;
    }
}

int day23()
{
    READ_INPUT("input");
    for_each_line(parse_line);
    int count = 0;
    for (int i = 0; i < 26; i++)
    {
        int t_node = i + ('t' - 'a')*26;
        if (!present[t_node])
            continue;
        for (int y = 0; y < ALPHA; y++)
        {
            if (!present[y])
                continue;
            int check1 = link[y][t_node] | link[t_node][y];
            if (!check1)
                continue;
            for (int z = y + 1; z < ALPHA; z++)
            {
                if (!present[z])
                    continue;
                if (y / 26 == 19 && y < t_node)
                    continue;
                if (z / 26 == 19 && z < t_node)
                    continue;                    
                int check2 = link[z][t_node] | link[t_node][z];
                int check3 = link[y][z];
                if (check2 && check3)
                    count++;
            }
        }
    }
    printf("%d\n", count);
    int *r = calloc(ALPHA, sizeof(int));
    int *x = calloc(ALPHA, sizeof(int));
    bron_kerbosch(r, present, x, 0);
    free(r);
    free(x);
    return 0;
}