#include "advent.h"
#include "hashmap.h"

long *nums;
size_t n_nums;

typedef struct {
    long key;
    long value;
} LongHash;

static int long_hash_eq(void *p, void *q)
{
    LongHash *a = (LongHash *)p;
    LongHash *b = (LongHash *)q;
    return a->key == b->key;
}

unsigned long_hash_hash(void *p) 
{
    return ((LongHash *)p)->key % HASH_SIZE;
}

static void parse_line(char *line)
{
    n_nums = scan_longs(line, &nums);
}

static void hash_save(LongHash *kv, HashMap *hash2, long save)
{
    LongHash *try;
    LongHash bar = {.key = save, .value = kv->value};
    if ((try = hash_add(hash2, &bar)) != NULL)
    {
        try->value += kv->value;
    }
}

static void replace(LongHash *kv, HashMap *b)
{
    long el = kv->key;
    if (el == 0)
        hash_save(kv, b, 1);
    else
    {
        long mul = 1;
        long exp = 0;
        while (mul <= el) {
            mul *= 10;
            exp++;
        }
        if (exp % 2 == 0)
        {
            int inc = 0;
            mul = 1;

            while (inc < exp / 2)
            {
                mul *= 10;
                inc++;
            }
            hash_save(kv, b, el / mul);
            hash_save(kv, b, el % mul);
        }
        else
            hash_save(kv, b, el * 2024);
    }
}

static void print_count(HashMap *hash1)
{
    long count = 0;
    LongHash *kv = hash_iterate(hash1);
    do
    {
        count += kv->value;
        kv = hash_iterate(NULL);
    } while (kv != NULL);
    printf("%ld\n", count);
}

static void solution()
{
    HashMap *hash1 = hash_init(long_hash_hash, long_hash_eq, sizeof(LongHash));
    HashMap *hash2 = hash_init(long_hash_hash, long_hash_eq, sizeof(LongHash));
    for (size_t i = 0; i < n_nums; i++)
    {
        LongHash xy = {.key = nums[i], .value = 1};
        hash_add(hash1, &xy); 
    }
    free(nums);

    for (int i = 0; i < 75; i++)
    {
        if (i == 25)
        {
            print_count(hash1);
        }

        LongHash *kv = hash_iterate(hash1);
        do
        {
            replace(kv, hash2);
            kv = hash_iterate(NULL);
        } while (kv != NULL);

        HashMap *tmp = hash1;
        hash1 = hash2;
        hash2 = tmp;
        hash_flush(hash2);
    }
    print_count(hash1);

    hash_free(hash1);
    hash_free(hash2);
}

int day11()
{
    READ_INPUT("input");
    for_each_line(parse_line);
    solution();
    return 0;
}