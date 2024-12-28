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
            for (int z = y + 1; z < ALPHA; z++)
            {
                if (!present[z])
                    continue;
                if (y / 26 == 19 && y < t_node)
                    continue;
                if (z / 26 == 19 && z < t_node)
                    continue;                    
                int check1 = link[y][t_node] | link[t_node][y];
                int check2 = link[z][t_node] | link[t_node][z];
                int check3 = link[y][z];
                if (check1 && check2 && check3)
                    count++;
            }
        }
    }
    printf("%d\n", count);
    return 0;
}