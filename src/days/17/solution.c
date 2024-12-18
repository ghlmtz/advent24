#include "advent.h"
#include "dyn_arr.h"

typedef struct
{
    long a;
    long b;
    long c;
    int *prog;
    int length;
    int ip;
    int *outputs;
    int n_outputs;
} Machine;

static int combo(Machine *machine)
{
    int op = machine->prog[machine->ip+1];
    switch (op)
    {
        case 4:
            return machine->a;
        case 5:
            return machine->b;
        case 6:
            return machine->c;
        default:
            return op;
    }
}

static void run_machine(Machine *machine)
{
    while (machine->ip < machine->length)
    {
        int op = machine->prog[machine->ip+1];
        switch (machine->prog[machine->ip])
        {
            case 0:
                machine->a = machine->a >> combo(machine);
                break;
            case 1:
                machine->b ^= op;
                break;
            case 2:
                machine->b = combo(machine) % 8;
                break;
            case 3:
                if (machine->a != 0)
                {
                    machine->ip = op;
                    continue;
                }
                break;
            case 4:
                machine->b = machine->b ^ machine->c;
                break;
            case 5:
                machine->outputs[machine->n_outputs++] = combo(machine) % 8;
                break;
            case 6:
                machine->b = machine->a >> combo(machine);
                break;
            case 7:
                machine->c = machine->a >> combo(machine);
                break;
        }
        machine->ip += 2;
    }
}

static long recurse(Machine *machine, DynArr **lookup, char *s, int index)
{
    if (index >= machine->length)
        return strtol(s, NULL, 8);

    DynArr *arr = lookup[machine->prog[machine->length - index - 1]];
    for(size_t i = 0; i < arr->length; i++)
    {
        char *oct_string = (char *)dyn_arr_get(arr, i);
        if(strncmp(oct_string, s + index, 3) == 0)
        {
            s[3 + index] = oct_string[3];
            long result = recurse(machine, lookup, s, index + 1);
            if (result != 0)
                return result;
        }
    }
    return 0;
}

int day17()
{
    char *buffer = read_input("input");
    int *nums;
    int n = scan_ints(buffer, &nums);
    int i;
    
    Machine machine = {.a = nums[0], .b = nums[1], .c = nums[2],
        .prog = nums + 3, .length = n - 3, .ip = 0,
        .outputs = malloc(sizeof(int) * n), .n_outputs = 0 };
    
    run_machine(&machine);
    for (i = 0; i < machine.n_outputs - 1; i++)
        printf("%d,", machine.outputs[i]);
    printf("%d\n", machine.outputs[i]);

    DynArr *lookup[8];
    for (i = 0; i < 8; i++)
        lookup[i] = dyn_arr_alloc(sizeof(char *));

    for (i = 0; i < 4096; i++)
    {
        machine.a = i;
        machine.b = 0;
        machine.c = 0;
        machine.ip = 0;
        machine.n_outputs = 0;
        run_machine(&machine);
        char oct_string[5] = {0};
        sprintf(oct_string, "%04o", i);
        dyn_arr_add(lookup[machine.outputs[0]], oct_string);
    }
    char s[20] = {0};
    s[0] = '0';
    s[1] = '0';
    s[2] = '0';
    printf("%ld\n", recurse(&machine, lookup, s, 0));

    for (i = 0; i < 8; i++)
        dyn_arr_free(lookup[i]);
    free(machine.outputs);
    free(nums);
    free(buffer);
    return 0;
}