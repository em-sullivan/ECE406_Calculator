/*
 * Eric Sullivan & Elizabeth Willard
 * 05/05/2020
 * Stack Functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

struct stack *init_stack(uint32_t s_size, uint32_t e_size)
{
    // Allocate memory for struct
    struct stack *st = (struct stack*)malloc(sizeof(struct stack));
    if (st == NULL) {
        printf("Error! Not enough memory!\n");
        return NULL;
    }
    
    st->maxsize = s_size;
    st->element_size = e_size;
    st->top = 0;

    // Allocate memory for data of stack
    st->data = (uint8_t*)malloc(e_size * s_size);
    if(st->data == NULL) {
        printf("Error! Not enough memory!\n");
        free(st);
        return NULL;
    }

    return st;

}

uint32_t stack_size(struct stack *st)
{
    return st->top;
}

int8_t stack_isempty(struct stack *st)
{
    return st->top == 0;
}

int8_t stack_isfull(struct stack *st)
{
    return st->top == (st->maxsize - 1);
}

int8_t push(struct stack *st, void *val)
{
    uint32_t i, index;

    //Checks to see if the stack is full
    if (stack_isfull(st)) {
        printf("Stack overflow\n");
        return 1;
    }

    index = (st->top) * st->element_size;

    for (i = 0; i < st->element_size; i++) {
        *(st->data + index + i) = *((uint8_t *)val + i);
    }

    st->top++;
    //st->data[++st->top] = val;
    return 0;
}

int8_t pop(struct stack *st, void *ret)
{
    uint8_t index, i;
    // Checks if stack is empty
    if (stack_isempty(st)) {
        printf("Stack Underflow\n");
        return 1;
    }

    //memset(ret, 0, sizeof(st->element_size));
    index = (st->top) * st->element_size;
    for (i = 0; i < st->element_size; i++) {
         *(st->data + index + i) = *((uint8_t *)ret + i);
    }

    st->top--;
    return 0;

    //return st->data[st->top--];
}

void * peek(struct stack *st)
{
    // Looks at the top of the stack
    return st->data[st->top];
}

void free_stack(struct stack *st)
{
    // Checks to be sure its safe to free memory
    if (st == NULL) {
        return;
    }
    
    // Free stack data
    if (st-> data != NULL)
        free(st->data);
    
    free(st); 
}