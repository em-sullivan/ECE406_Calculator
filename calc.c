/*
 * Eric Sullivan
 * 05/09/2020
 * Calculator functions header
 */

#include <stdio.h>
#include <ctype.h>
#include "stack.h"
#include "calc.h"


int eval_postfix(char *exp)
{
    char *cp;
    int tempnum, num1, num2, num3;
    struct stack *st;

    st = init_stack(MAXSTACK);
    cp = exp;
    while (*cp != '\0') {
        // Checks if value in string is a number
        if(isdigit(*cp)) {
            tempnum = (int)*cp - 48;
            push(st, tempnum);

        } else {
            num1 = pop(st);
            num2 = pop(st);

            switch(*cp) {
                case '+':
                    num3 = num1 + num2;
                    break;
                case '-':
                    num3 = num1 - num2;
                    break;
                case '*':
                    num3 = num1 * num2;
                    break;
                case '/':
                    num3 = num1 / num2;
                    break;
                default: break;
            }

            push(st, num3);
        }
        cp++;
    }
    tempnum = pop(st);
    free_stack(st);
    return tempnum;
   

}
