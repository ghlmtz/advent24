#include "advent.h"

typedef struct mem_block {
    int idx;
    int len;
    struct mem_block *prev;
    struct mem_block *next;
} MemoryBlock;

static long part1 = 0;
static long part2 = 0;

static int search(MemoryBlock *head, MemoryBlock *end, int dec_id)
{
    MemoryBlock *prev = NULL;
    while (head->idx > -1 || head->len < end->len)
    {
        prev = head;
        head = head->next;
        if (head == NULL || head == end) 
        {
            return dec_id - 1;
        }
    }
    if (head->len == end->len)
    {
        head->idx = end->idx;
        end->idx = -1;
    }
    else
    {
        MemoryBlock *new_blk = malloc(sizeof(MemoryBlock));
        new_blk->idx = end->idx;
        new_blk->len = end->len;
        if (prev != NULL)
            prev->next = new_blk;
        new_blk->prev = prev;
        new_blk->next = head;
        head->len = head->len - end->len;
        end->idx = -1;
    }
    return dec_id - 1;
}

static void parse_line2(char *line)
{
    MemoryBlock *head = NULL;
    MemoryBlock *tail = NULL;
    int idx = 0;
    int line_idx = 0;
    int count = 0;
    while (line[line_idx] != '\0') 
    {
        if (line[line_idx] != 0)
        {
            MemoryBlock *blk = malloc(sizeof(MemoryBlock));
            blk->idx = count % 2 == 0 ? idx : -1;
            blk->len = line[line_idx] - '0';
            blk->next = NULL;
            if (head == NULL) {
                head = blk;
                blk->prev = NULL;
                tail = blk;
            }
            else {
                tail->next = blk;
                blk->prev = tail;
                tail = blk;
            }
        }
        if (count % 2  == 0)
            idx++;
        line_idx++;
        count++;
    }

    MemoryBlock *end = tail;
    MemoryBlock *start = head;
    int dec_id = end->idx;
    int inc_id = 0;
    while (end != NULL && dec_id > 0)
    {
        dec_id = search(head, end, dec_id);
        while (head->next->idx != -1 || head->next->len == 0) {
            if (head->idx == inc_id)
                inc_id++;
            head = head->next;
        }
        if (dec_id <= inc_id)
            break;
        while(end->idx != dec_id)
            end = end->prev;
    }
    count = 0;
    while (start != NULL)
    {
        for (int i = 0; i < start->len; i++, count++) {
            if (start->idx != -1)
                part2 += count * start->idx;
        }
        end = start->next;
        free(start);
        start = end;
    }
}

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

    parse_line2(line);
}

int day9()
{
    READ_INPUT("input");
    for_each_line(parse_line);
    printf("%ld\n", part1);
    printf("%ld\n", part2);
    return 0;
}