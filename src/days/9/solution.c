#include "advent.h"

typedef struct mem_block {
    int idx;
    int len;
    struct mem_block *prev;
    struct mem_block *next;
} MemoryBlock;

static void search(MemoryBlock *head, MemoryBlock *end)
{
    MemoryBlock *prev = head;
    head = head->next;
    while (head->idx > -1 || head->len < end->len)
    {
        prev = head;
        head = head->next;
        if (head == end) 
        {
            return;
        }
    }
    if (head->len == end->len) /* Perfect match, overwrite idx */
    {
        head->idx = end->idx;
    }
    else /* Shrink free space and add new index block */
    {
        MemoryBlock *new_blk = malloc(sizeof(MemoryBlock));
        prev->next = new_blk;

        new_blk->idx = end->idx;
        new_blk->len = end->len;
        new_blk->prev = prev;
        new_blk->next = head;
        
        head->len = head->len - end->len;
    }
    end->idx = -1; /* Free up moved block */
}

static void part2(char *line)
{
    MemoryBlock *head = NULL;
    MemoryBlock *tail = NULL;
    int line_idx = 0;
    while (line[line_idx] != '\0') 
    {
        if (line[line_idx] != 0)
        {
            MemoryBlock *blk = malloc(sizeof(MemoryBlock));
            blk->idx = line_idx % 2 == 0 ? (line_idx + 1) / 2 : -1;
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
        line_idx++;
    }

    MemoryBlock *end = tail;
    MemoryBlock *start = head;
    int dec_id = end->idx;
    int inc_id = 0;
    while (1)
    {
        search(head, end);
        dec_id--;
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
    int count = 0;
    long pt2 = 0;
    while (start != NULL)
    {
        for (int i = 0; i < start->len; i++, count++) {
            if (start->idx != -1)
                pt2 += count * start->idx;
        }
        end = start->next;
        free(start);
        start = end;
    }
    printf("%ld\n", pt2);
}

static void part1(char *line)
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
    long pt1 = 0;
    while (1) {
        if (enormous[start_idx] != -1) {
            pt1 += count * enormous[start_idx];
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
                pt1 += count * backwards[end_idx];
            }
            end_idx++;
        }

        if(start_idx >= idx - end_idx)
            break;
        count++;
    }

    free(enormous);
    free(backwards);

    printf("%ld\n", pt1);

    part2(line);
}

int day9()
{
    READ_INPUT("input");
    for_each_line(part1);
    return 0;
}