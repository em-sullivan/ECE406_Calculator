/*
 * Eric Sullivan
 * 05/09/2020
 * Calculator functions header
 */

#ifndef CALC_H
#define CALC_H

#include <stdint.h>

enum modes {
    DEC = 0,
    BIN = 1,
    HEX = 2,
    OCT = 3
};

float eval_postfix(char * expression);
float express(char op, float n1, float n2);
int precd(char op);
int infix_to_postfix(char *inexp, char *postexp);
void print_binary(int val);
/*
 * NOTE: I'm note sure if we are able to use printf with the LCD screen.
 * IF we can rediret stdout to it (which it looks like we may be able to),
 * then converting ints to octal/hex is relatively easy. IF not, then functions
 * will be written for those.
 */
void print_hex(int val);
void print_oct(int val);
void print_mode(int val, int mode);
#endif //CALC_H