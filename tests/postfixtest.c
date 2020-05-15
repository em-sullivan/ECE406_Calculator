/*
 * Eric Sullivan and Elizabeth Willard
 * 05/10/2020
 * Test program for postix evaluation
 */

#include <stdio.h>
#include "../calc.h"

int main(int argc, char *argv[])
{
    int ans;

    if (argc < 2) {
        ans = eval_postfix("2,4,5,+*");
        printf("245+* is %d\n", ans);
    } else {
        ans = eval_postfix(argv[1]);
        printf("%s is %d\n", argv[1], ans);
    }

    return 0;
}