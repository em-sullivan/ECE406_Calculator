/*
 * Eric Sullivan & Elizabeth Willard
 * 05/05/2020
 * Stack Functions
 */

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

struct stack *init_stack(int size)
{
    // Allocate memory for struct
    struct stack *st = (struct stack*)malloc(sizeof(struct stack));
    if (st == NULL) {
        printf("Error! Not enough memory!\n");
        return NULL;
    }

    st->maxsize = size;
    st->top = -1;
    st->data = (int*)malloc(sizeof(int) * size);
    if(st->data == NULL) {
        printf("Error! Not enough memory!\n");
        free(st);
        return NULL;
    }

    return st;

}

int stack_size(struct stack *st)
{
    return st->top + 1;
}

int stack_isempty(struct stack *st)
{
    return st->top == -1;
}

int stack_isfull(struct stack *st)
{
    return st->top == (st->maxsize - 1);
}

int push(struct stack *st, int val)
{
    //Checks to see if the stack is full
    if (stack_isfull(st)) {
        printf("Stack overflow\n");
        return 1;
    }

    st->data[++st->top] = val;
    return 0;
}

int pop(struct stack *st, int *val)
{
    // Checks if stack is empty
    if (stack_isempty(st)) {
        printf("Stack Underflow\n");
        return -1;
    }

    *val = st->data[st->top--];
    return 0;
}

int peek(struct stack *st)
{
    return st->data[st->top];
}

void free_stack(struct stack *st)
{
    if (st == NULL) {
        return;
    }

    free(st->data);
    free(st); 
}
