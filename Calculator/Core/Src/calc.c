/*
 * Eric Sullivan
 * 05/09/2020
 * Calculator functions header
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "stack.h"
#include "calc.h"

float eval_postfix(char *exp)
{
    char *cp;
    float num1, num2, num3;
    char temp[20];
    struct stack *st;
    int negative = 0;

    st = init_stack(MAXSTACK);
    if (st == NULL) {
        printf("Could not create struct!\n");
        return 1;
    }
    
    // Sets char pointer to start of epxression
    cp = exp;
    // Number buffer
    strcpy(temp, "");

    while (*cp != '\0') {
        // Checks if value in string is a number, adds
        // digit to temp string
        if (isdigit(*cp) || *cp == '.') {
            strncat(temp, cp, 1);
        
        // Stores number into stack 
        } else if (*cp == ',') {
            num1 = strtof(temp, NULL);
            if (negative)
                num1 *= -1.0;
            
            // Push number to stack, reset temp string
            // and negative val
            push(st, num1);
            strcpy(temp, "");
            negative = 0;
            
        // Makes number negative
        } else if (*cp == '_') {
            negative ^= 1;
        // Performs operation and stores back into stack
        } else {
            // Pops the last two numbers off the stack, performs
            // math operation, then returns answer to stack
            num1 = pop(st);
            num2 = pop(st);
            num3 = express(*cp, num1, num2);
            push(st, num3);
        }
        cp++;
    }

    // Final result
    num1 = pop(st);
    free_stack(st);
    return num1;
}

float express(char op, float n1, float n2)
{
    float ans;

    switch(op) {
        case '+':
            ans = n1 + n2;
            break;
        case '-':
            ans = n2 - n1;
            break;
        case '*':
            ans = n1 * n2;
            break;
        case '/':
            ans = n2 / n1;
            break;
        case '&':
            ans = (int)n1 & (int)n2;
            break;
        case '|':
            ans = (int)n1 | (int)n2;
            break;
        case '^':
            ans = (int)n1 ^ (int)n2;
            break;
        default:
            ans = 0; 
            break;
    }

    return ans;      
}

int precd(char op)
{
    switch(op) {
        case '&':
            return 1;
        case '^':
            return 2;
        case '|':
            return 3;
        case '+':
        case '-':
            return 4;
        case '*':
        case '/':
            return 5;
        default: break;
    }
    
    return -1;
}

int infix_to_postfix(char *inexp, char *postexp)
{
    struct stack *st;
    char *infixp;
    char *postfixp;
    
    st = init_stack(MAXSTACK);
    if (st == NULL) {
        printf("Could not create struct!\n");
        return 1;
    }

    // Initialises pointers to input and output expression
    infixp = inexp;
    postfixp = postexp;

    while(*infixp != '\0') {
        if (isdigit(*infixp) || *infixp == '_' || *infixp == '.') {
            *postfixp = *infixp;
            postfixp++;
        
        // Pushes parenthesis to stack
        } else if (*infixp == '(') {
            push(st, *infixp);

        // At end of parenthesis, pop values off stack
        // unitl a ( appears
        } else if (*infixp == ')') {
            while (!stack_isempty(st) && peek(st) != '(') {
                *postfixp = pop(st);
                postfixp++;
            }

            // Checking if a ( was never reached
            if(!stack_isempty(st) && peek(st) != '(') {
                return 2;
            } else {
                pop(st);
            }

        } else {
            // Determines the order of operators (_, -, etc.)
            while(!stack_isempty(st) && precd(*infixp) <= precd(peek(st))) {
                *postfixp = pop(st);
                postfixp++;
            }
            push(st, *infixp);
        }
        
        infixp++;

        // Adds commas to seperate values
        if ((!(isdigit(*infixp)) && *infixp != '.') && isdigit(*(infixp-1))) {
            *postfixp = ',';
            postfixp++;
        }
    }

    // Pop remaing chars on the stack
    while (!stack_isempty(st)) {
        *postfixp = pop(st);
        postfixp++;
    }
    
    *postfixp = '\0';
    free_stack(st);
    return 0;
}

/*
 * These functions will not be used in the final
 * version of the project, they are here for
 * testing purposes
 */
void print_binary(int val)
{
    int i;
    int leading_zero = 0;
    unsigned int mask = 0x80000000;

    for (i = 0; i < 32; i++) {
        if (val & (mask >> i)) {
            printf("1");
            leading_zero = 1;
        } else {
            if (leading_zero)
                printf("0");
        }    
    }

    printf("\n");
}

void print_mode(int val, int mode)
{
    switch(mode) {
            case BIN:
            print_binary(val);
            break;
        case OCT:
            printf("%o\n", val);
            break;
        case HEX:
            printf("%X\n", val);
            break;
        case DEC:
        default:
            printf("%d\n", val);
    }
}
