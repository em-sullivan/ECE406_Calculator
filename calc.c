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
            pop(st, &num1);
            pop(st, &num2);

            num3 = express(*cp, num1, num2);

            push(st, num3);
        }
        cp++;
    }
    pop(st, &tempnum);
    free_stack(st);
    return tempnum;
}

int express(char op, int n1, int n2)
{
    int ans;

     switch(op) {
                case '+':
                    ans = n1 + n2;
                    break;
                case '-':
                    ans = n1 - n2;
                    break;
                case '*':
                    ans = n1 * n2;
                    break;
                case '/':
                    ans = n1 / n2;
                    break;
                default: break;
    }

    return ans;
        
}