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
    int popval = 0;
    int stackSize;
    int i; // Index

    st = init_stack(MAXSTACK, sizeof(int));

    printf("Hello\n");
    for (i = 1; i < 5; i++) {
        push(st, (int)i);
    }

    //printf("%f\n", peek(st));
    stackSize = stack_size(st);
    i = 0;
    for (i = 0; i < 4; i++) {
        pop(st, (int) popval);
        printf("%d\n", popval);
        printf("%d\n", i);
    }

    free_stack(st);
    return 0;
}
