/*
 * Eric Sullivan
 * 05/05/2020
 * Test file for stack
 */

#include <stdio.h>
#include "stack.h"

int main (int argc, char *argv[])
{
    struct stack *st; //Stack
    float popval;
    int stackSize;
    int i; // Index

    st = init_stack(MAXSTACK);

    push(st, 1);
    push(st, 2);
    push(st, 3);
    push(st, 4);

    printf("%f\n", peek(st));
    stackSize = stack_size(st);
    for (i = 0; i < stackSize; i++) {
        pop(st, &popval);
        printf("%f\n", popval);
    }

    free_stack(st);
    return 0;
}
