#include "advent.h"
#include "dyn_arr.h"

DynArr *locks;
DynArr *keys;

static int cols;
static int rows;

static void parse_lock_or_key(char *buffer)
{
    int is_key = buffer[0] == '.' ? 1 : 0;
    int *nums = calloc(cols, sizeof(int));
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols - 1; j++)
        {
            char ch = buffer[i * cols + j];
            if (is_key && ch == '.')
                nums[j]--;
            if (!is_key && ch == '#')
                nums[j]++;
        }
    }
    if (is_key)
        dyn_arr_add(keys, &nums);
    else
        dyn_arr_add(locks, &nums);
}

int day25()
{
    locks = dyn_arr_alloc(sizeof(int *));
    keys = dyn_arr_alloc(sizeof(int *));
    char *buffer = read_input("input");
    rows = count_to_blank(0) - 1;
    cols = char_index(buffer, '\n') + 1;
    for (size_t i = 0; i < strlen(buffer); i += (cols * rows) + 1)
        parse_lock_or_key(buffer + i);
    int count = 0;
    for (size_t lock = 0; lock < locks->length; lock++)
    {
        int *lock_test = DYN_ARR_GET(int *, locks, lock);
        for (size_t key = 0; key < keys->length; key++)
        {
            int *key_test = DYN_ARR_GET(int *, keys, key);
            int good = 1;
            for (int i = 0; i < cols - 1; i++)
            {
                if (lock_test[i] + key_test[i] > 0) {
                    good = 0;
                    break;
                }
            }
            if (good)
                count++;
        }
        free(lock_test);
    }
    printf("%d\n", count);
    for (size_t key = 0; key < keys->length; key++)
    {
        int *key_test = DYN_ARR_GET(int *, keys, key);
        free(key_test);
    }
    dyn_arr_free(locks);
    dyn_arr_free(keys);
    free(buffer);
    return 0;
}