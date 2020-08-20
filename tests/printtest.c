#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calc.h"

int main(int argc, char *argv[])
{
    char exp[40];;
    char ans[40];
    int result;
    int mode;

    if (argc < 2) {
        strcpy(exp, "5*(2+3)");
        mode = DEC;
    } else if (argc < 3) {
        strcpy(exp, argv[1]);
        mode = DEC;
    } else {
        strcpy(exp, argv[1]);
        mode = atoi(argv[2]);
    }

    infix_to_postfix(exp, ans);
    printf("%s is %s\n", exp, ans);
    result = eval_postfix(ans);
    print_mode(result, mode);

    return 0;
}
