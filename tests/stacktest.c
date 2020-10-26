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
    float popval = 0;
    float data[] = {4.0, 3.0, 2.0, 1.0};
    int stackSize;
    int i; // Index

    st = init_stack(MAXSTACK, sizeof(int));

    for (i = 0; i < 4; i++) {
        push(st, &data[i]);
    }

    printf("%f\n", *(float*)peek(st));
    stackSize = stack_size(st);
    i = 0;
    for (i = 0; i < 4; i++) {
        popval = *(float *)pop(st);
        printf("%f\n", popval);
    }

    free_stack(st);
    return 0;
}
