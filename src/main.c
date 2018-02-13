
#include <stdio.h>
#include "script.h"

// This snippet was taken from raylib's rlgl.c
char *loadtext(const char *fileName) 
{
    FILE *textFile;
    char *text = NULL;
    int count = 0;
    if (fileName != NULL) {
        textFile = fopen(fileName,"rt");
        if (textFile != NULL) {
            fseek(textFile, 0, SEEK_END);
            count = ftell(textFile);
            rewind(textFile);
            if (count > 0) {
                text = (char *)malloc(sizeof(char)*(count + 1));
                count = fread(text, sizeof(char), count, textFile);
                text[count] = '\0';
            }
            fclose(textFile);
        }
        else printf("ERROR: NO FILE\n");
    }
    return text;
}

int test_func(baslike_t* s) {
    scriptoutput(s, "TEST FUNCT\n");
    return -1;
}

int hash_func(baslike_t* s) {
    int x = s->args[0];
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    s->ret = x;
    return -1;
}

int main(int argc, char** argv)
{
    
    if (argc < 2) {
        printf("ERROR: NO SCRIPT\n");
        return 1;
    }
    if (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version")) {
        printf("BASLIKE VER 0.9.3\n");
        return 0;
    }
    char* str = loadtext(argv[1]);
    if (!str) return 1;
    baslike_t script;
    linkfunction(&script, test_func, "TEST");
    linkfunction(&script, hash_func, "HASH");
    
    execute(&script, str);
    printf("-- STACK INFO --\n");
    printf("LABELS : %d\n", script.labelsize);
    printf("OPS    : %d\n", script.stacksize);
    printf("--   OUTPUT   --\n");
    printf("%s", script.output);
    if (script.failed) {
        printf("--   FAILED   --\n");
    } else {
        printf("--   SUCCESS  --\n");
    }
    
    return 0;
}
