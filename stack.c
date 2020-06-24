/*
 * Eric Sullivan & Elizabeth Willard
 * 05/05/2020
 * Stack Functions
 */

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

struct stack *init_stack(uint32_t size)
{
    // Allocate memory for struct
    struct stack *st = (struct stack*)malloc(sizeof(struct stack));
    if (st == NULL) {
        printf("Error! Not enough memory!\n");
        return NULL;
    }

    st->maxsize = size;
    st->top = -1;

    // Allocate memory for data of stack
    st->data = (int32_t*)malloc(sizeof(int32_t) * size);
    if(st->data == NULL) {
        printf("Error! Not enough memory!\n");
        free(st);
        return NULL;
    }

    return st;

}

uint32_t stack_size(struct stack *st)
{
    return st->top + 1;
}

int8_t stack_isempty(struct stack *st)
{
    return st->top == -1;
}

int8_t stack_isfull(struct stack *st)
{
    return st->top == (st->maxsize - 1);
}

int8_t push(struct stack *st, int32_t val)
{
    //Checks to see if the stack is full
    if (stack_isfull(st)) {
        printf("Stack overflow\n");
        return 1;
    }

    st->data[++st->top] = val;
    return 0;
}

int8_t pop(struct stack *st, int32_t *popval)
{
    // Checks if stack is empty
    if (stack_isempty(st)) {
        printf("Stack Underflow\n");
        return -1;
    }

    *popval = st->data[st->top--];
    return 0;
}

int32_t peek(struct stack *st)
{
    return st->data[st->top];
}

void free_stack(struct stack *st)
{
    // Checks to be sure its safe to free memory
    if (st == NULL) {
        return;
    }
    if (st-> data != NULL)
        free(st->data);
    
    free(st); 
}
