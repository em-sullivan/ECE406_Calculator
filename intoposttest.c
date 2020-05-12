/*
 * Eric Sullivan and Elizabeth Willard
 * 05/10/2020
 * Test program for postix evaluation
 */

#include <stdio.h>
#include "calc.h"

int main(int argc, char *argv[])
{
    char exp[] = "(2+2)";
    char ans[40];

    infix_to_postfix(exp, ans);

    printf("%s is %s\n", exp, ans);

    return 0;
}