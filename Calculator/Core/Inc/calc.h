/*
 * Eric Sullivan
 * 05/09/2020
 * Calculator functions header
 */

#ifndef CALC_H
#define CALC_H

#include <stdint.h>

int eval_postfix(char *expression, double *ans);
double express(char op, double n1,double n2);
int precd(char op);
int infix_to_postfix(char *inexp, char *postexp);
void print_binary(int val);
int convert_string(char *string, double *val);

#endif //CALC_H
