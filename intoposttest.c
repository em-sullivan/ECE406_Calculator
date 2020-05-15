/*
 * Eric Sullivan and Elizabeth Willard
 * 05/10/2020
 * Test program for postix evaluation
 */

#include <stdio.h>
#include <string.h>
#include "calc.h"

int main(int argc, char *argv[])
{
    char exp[40];;
    char ans[40];
    int result;

    if (argc < 2) {
        strcpy(exp, "5*(2+3)");
    } else {
        strcpy(exp, argv[1]);
    }

    infix_to_postfix(exp, ans);
    printf("%s is %s\n", exp, ans);
    result = eval_postfix(ans);
    printf("%d\n", result);



    return 0;
}