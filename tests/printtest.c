#include <stdio.h>
#include <stdlib.h>
#include "../calc.h"

int main(int argc, char *argv[])
{
    int i;

    if (argc < 2)
        i = 212;
    else
        i = atoi(argv[1]);
    
    print_mode(i, DEC);
    print_mode(i, BIN);
    print_mode(i, OCT);
    print_mode(i, HEX);

    print_hex(i);
    print_oct(i);
    return 0;
    
}