
#include <stdio.h>
#include <raylib.h>
#include "script.h"

int main(int argc, char** argv)
{
    char* str = LoadText("fib.b");
    printf("executing: fib.b\n");
    printf("---------------\n");
    execute(str);
    printf("---------------\n");
    stackinfo();
    return 0;
}
