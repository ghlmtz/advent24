#include "advent.h"

static long part1 = 0;
static long part2 = 0;

static void parse_line(char *line)
{
    int *enormous = malloc(sizeof(int) * 100000);
    int *backwards = malloc(sizeof(int) * 100000);
    int line_idx = 0;
    int idx = 0;
    int id = 0;
    while (line[line_idx] != '\0')
    {
        int count = line[line_idx] - '0';
        for (int i = 0; i < count; i++)
            enormous[idx++] = line_idx % 2 == 0 ? id : -1;
        if (line_idx % 2 == 0)
            id++;
        line_idx++;
    }
    for (int i = 0; i < idx; i++)
    {
        backwards[i] = enormous[idx - i - 1];
    }

    int start_idx = 0;
    int end_idx = 0;
    int count = 0;
    while (1) {
        if (enormous[start_idx] != -1) {
            part1 += count * enormous[start_idx];
            start_idx++;
        }
        else {
            start_idx++;
            while(backwards[end_idx] == -1)
            {
                end_idx++; 
                if(start_idx >= idx - end_idx)
                    break;
            } 
            if(backwards[end_idx] != -1)
            {
                part1 += count * backwards[end_idx];
            }
            end_idx++;
        }

        if(start_idx >= idx - end_idx)
            break;
        count++;
    }

    free(enormous);
    free(backwards);
}

int day9()
{
    READ_INPUT("input");
    for_each_line(parse_line);
    printf("\n%ld\n", part1);
    printf("%ld\n", part2);
    return 0;
}