/*
 * Eric Sullivan
 * 05/09/2020
 * Calculator functions header
 */

#ifndef CALC_H
#define CALC_H

#include <stdint.h>

/* 
 * Evlaute postfix expression, save answer into 
 * ans pointer. Returns 0 if executed correctly,
 * or it returns a postive int if the expression
 * was invalid
 */
int32_t eval_postfix(char *expression, double *ans);

/*
 * Peforms mathmatical operation on two input numbers, then
 * retunrs the answer. For normal operators +, -, / and * it performs
 * double math but for the bit operators (>>, <<, &, |, ^) it casts
 * the double as a 32bit interger, which truncates the decimale value
 */
double express(char op, double n1,double n2);

/*
 * Returns the precidence of the input operator. This is used in the
 * infix_to_postfix function to determine the precidence of an operator
 */
int32_t precd(char op);


/*
 * Convert an infix expression into a postfix expression. Returns
 * 0 if performed correclty, anyother value means an error occured
 */
int32_t infix_to_postfix(char *inexp, char *postexp);

/*
 * Checks to see if the currenct passed char c is contatined in the string
 * passed as key. If it is, return 1 else return 0
 */
int32_t is_char_in_list(char *key, char c);

/*
 * Converts string into a double value. This can convert a normal
 * decimale value, a binary value, octal value or hexadecimale value into
 * a double. Returns 0 if performed correctly, anything else means an error occrued.
 */
int32_t convert_string(char *string, double *val);

#endif //CALC_H
