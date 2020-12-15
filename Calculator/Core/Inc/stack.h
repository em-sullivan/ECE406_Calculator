/*
 * Eric Sullivan & Elizabeth Willard
 * 05/05/2020
 * Header for Stack Functions
 */

#ifndef STACK_H
#define STACK_H

#include <stdint.h>

#define MAXSTACK 34
struct stack {
    uint32_t maxsize;
    int32_t top;
    double *data;
};

struct stack *init_stack(uint32_t size);
uint32_t stack_size(struct stack *st);
int8_t stack_isempty(struct stack *st);
int8_t stack_isfull(struct stack *st);
int8_t push(struct stack *st, double val);
double pop(struct stack *st);
double peek(struct stack *st);
void free_stack(struct stack *st);

#endif //STACK_H
