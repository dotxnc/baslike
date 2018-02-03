
#include <stdio.h>
#include <raylib.h>
#include "script.h"

int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("ERROR: NO SCRIPT\n");
        return 1;
    }
    char* str = LoadText(argv[1]);
    printf("executing: fib.b\n");
    printf("---------------\n");
    execute(str);
    printf("---------------\n");
    stackinfo();
    return 0;
}
