/*
 * Eric Sullivan & Elizabeth Willard
 * 05/05/2020
 * Header for Stack Functions
 */

#ifndef STACK_H
#define STACK_H

#define MAXSTACK 20
#define MAXSTRING 10

struct stack {
    int maxsize;
    int top;
    int *data;
};

struct stack *init_stack(int size);
int stack_size(struct stack *st);
int stack_isempty(struct stack *st);
int stack_isfull(struct stack *st);
void push(struct stack *st, int val);
int pop(struct stack *st);
int peek(struct stack *st);
void free_stack(struct stack *st);

#endif //STACK_H
