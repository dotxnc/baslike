
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

int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("ERROR: NO SCRIPT\n");
        return 1;
    }
    if (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version")) {
        printf("BASLIKE VER 0.4\n");
        return 0;
    }
    char* str = loadtext(argv[1]);
    if (!str) return 1;
    printf("executing: %s\n", argv[1]);
    printf("---------------\n");
    execute(str);
    printf("---------------\n");
    stackinfo();
    return 0;
}
