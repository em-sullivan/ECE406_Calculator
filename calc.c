/*
 * Eric Sullivan
 * 05/09/2020
 * Calculator functions header
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "stack.h"
#include "calc.h"


int eval_postfix(char *exp)
{
    char *cp;
    int tempnum, num1, num2, num3;
    struct stack *st;

    st = init_stack(MAXSTACK);
    if (st == NULL) {
        printf("Could not create struct!\n");
        return 1;
    }

    cp = exp;
    tempnum = 0;
    while (*cp != '\0') {
        // Checks if value in string is a number
        if (isdigit(*cp)) {
            tempnum = tempnum * 10 + ((int)*cp - 48);
        } else if (*cp == ',') {
            push(st, tempnum);
            tempnum = 0;
        // Performs operation and stores back into stack
        } else {
            pop(st, &num1);
            pop(st, &num2);
            num3 = express(*cp, num1, num2);
            push(st, num3);
        }
        cp++;
    }

    // Final result
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
        case '&':
            ans = n1 & n2;
            break;
        case '|':
            ans = n1 | n2;
            break;
        case '^':
            ans = n1 ^ n2;
            break;
        default: break;
    }

    return ans;      
}

int precd(char op)
{
    switch(op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        default: break;
    }

    return -1;
}

int infix_to_postfix(char *inexp, char *postexp)
{
    struct stack *st;
    char *inp;
    char *posp;
    int noth;
    
    // Allocates memory for stack
    st = init_stack(MAXSTACK);
    if (st == NULL) {
        printf("Could not create struct!\n");
        return 1;
    }

    // Initialises points to input and output expression
    inp = inexp;
    posp = postexp;

    while(*inp != '\0') {
        if (isdigit(*inp)) {
            *posp = *inp;
            posp++;
        
        // Handles values in parentheses
        } else if (*inp == '(') {
            push(st, *inp);
        } else if (*inp == ')') {
            while (!stack_isempty(st) && peek(st) != '(') {
                pop(st, &noth);
                *posp = noth;
                posp++;
            }

            if(!stack_isempty(st) && peek(st) != '(') {
                return 2;
            } else {
                pop(st, &noth);
            }

        } else {
            while(!stack_isempty(st) && precd(*inp) <= precd(peek(st))) {
                pop(st, &noth);
                *posp = noth;
                posp++;
            }
            push(st, *inp);
        }
        
        inp++;

        // Adds commas to seperate values
        if (!(isdigit(*inp)) && isdigit(*(inp-1))) {
            *posp = ',';
            posp++;
        }
    }

    while (!stack_isempty(st)) {
        pop(st, &noth);
        *posp = noth;
        posp++;
    }
    *posp = '\0';
    free_stack(st);
    return 0;
}