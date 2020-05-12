/*
 * Eric Sullivan
 * 05/09/2020
 * Calculator functions header
 */

#ifndef CALC_H
#define CALC_H

int eval_postfix(char * expression);
int express(char op, int n1, int n2);
int precd(char op);
int infix_to_postfix(char *inexp, char *postexp);

#endif //CALC_H