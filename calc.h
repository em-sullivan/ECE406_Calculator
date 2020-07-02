/*
 * Eric Sullivan
 * 05/09/2020
 * Calculator functions header
 */

#ifndef CALC_H
#define CALC_H

#include <stdint.h>

float eval_postfix(char * expression);
float express(char op, float n1, float n2);
int precd(char op);
int infix_to_postfix(char *inexp, char *postexp);
void print_binary(float val);
void print_hex(float val);
void print_oct(float val);


#endif //CALC_H