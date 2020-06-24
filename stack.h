/*
 * Eric Sullivan & Elizabeth Willard
 * 05/05/2020
 * Header for Stack Functions
 */

#ifndef STACK_H
#define STACK_H

#include <stdint.h>

#define MAXSTACK 20
#define MAXSTRING 10

struct stack {
    uint32_t maxsize;
    int32_t top;
    int32_t *data;
};

struct stack *init_stack(uint32_t size);
uint32_t stack_size(struct stack *st);
int8_t stack_isempty(struct stack *st);
int8_t stack_isfull(struct stack *st);
int8_t push(struct stack *st, int32_t val);
int8_t pop(struct stack *st, int32_t *popval);
int32_t peek(struct stack *st);
void free_stack(struct stack *st);

#endif //STACK_H
