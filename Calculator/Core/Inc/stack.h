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

/*
 * Allocate the memory needed to form a stack
 * of doubles at the specified size. Returns NULL
 * if there is not enough memory
 */
struct stack *init_stack(uint32_t size);

/*
 * Returns The current size of the stack
 */
uint32_t stack_size(struct stack *st);

/*
 * Returns 1 (true) if the stack is empty, else
 * it returns 0
 */
int8_t stack_isempty(struct stack *st);

/*
 * Returns 1 (true) if the stack is full, else
 * it retunrs 0
 */
int8_t stack_isfull(struct stack *st);

/*
 * Pushes a value to the stack. Returns a non-zero
 * value if the stack is full
 */
int8_t push(struct stack *st, double val);

/*
 * Removes the top double on the stack, and returns it.
 * The user should call the stack_isempty function before
 * calling this function
 */
double pop(struct stack *st);

/*
 * Returns the current double at the top of the stack.
 * The user should call stack_isempty before calling
 * this function.
 */
double peek(struct stack *st);

/*
 * Free all of the memory allocated by the stack structure
 */
void free_stack(struct stack *st);

#endif //STACK_H
