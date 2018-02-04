#include "script.h"

void execute(char* text)
{
    populate(text);
    preprocess();
    for (opindex = 0; opindex < stacksize; opindex++)
    {
        int op = isop(stack[opindex]);
        doop(op);
        if (failed) break;
    }
}

void preprocess()
{
    for (int i = 0; i < stacksize; i++) {
        if (isop(stack[i]) == OP_DEF) {
            labels[labelsize] = i+1;
            labelsize++;
        }
    }
    printf("preprocessor found %d %s\n", labelsize, labelsize>1||labelsize==0?"labels":"label");
    printf("---------------\n");
}

void populate(char* text)
{
    int i;
    for (i=0;i<512;i++)memset(stack[i], '\0', 16);
    for (i=0;i<strlen(text);i++)if(text[i]=='\n')text[i]=' ';
    int index = 0;
    char *token = strtok(text, " \n");
    while(token) {
        bool ignore = false;
        for (i=0;i<strlen(token);i++) {
            if (islower(token[i])) {
                ignore = true;
                break;
            }
        }
        if (!ignore) {
            strcpy(stack[index], token);
            index++;
        }
        token = strtok(NULL, " ");
    }
    free(token);
    stacksize = index;
}

int isop(char* op)
{
    for (int i = 0; i < OPS; i++)
        if (!strcmp(op, ops[i])) return i;
    return OP_NON;
}

void doop(int op)
{
    switch (op)
    {
        case OP_NON: {
            printf("NON OPERATION (%s:%d)\n", stack[opindex], opindex);
        } break;
        case OP_MDS: {
            if (isop(stack[opindex+1]) == OP_MDX) mds = memory[mdx];
            else mds = atoi(stack[opindex+1]);
            opindex++;
        } break;
        case OP_IFE: {
            int els = -1;
            int enf = -1;
            for (int i = opindex; i < stacksize; i++) {
                int eop = isop(stack[i]);
                if (eop == OP_ENF) {
                    enf = i;
                    break;
                }
                if (eop == OP_ELS) {
                    els = i;
                }
            }
            if (enf == -1) {
                printf("ERROR: NO ENF\n");
                failed=true;
                break;
            }
            int m;
            bool jumped=false;
            if (isop(stack[opindex+1]) == OP_MDX) m = memory[mdx];
            else m = atoi(stack[opindex+1]);
            if (memory[mds] == atoi(stack[opindex+1])) {
                opindex+=2;
                int to = els > -1 ? els : enf;
                for (; opindex < to; opindex++) {
                    if (isop(stack[opindex])==OP_JMP) {
                        doop(isop(stack[opindex]));
                        jumped=true;
                        break;
                    }
                    doop(isop(stack[opindex]));
                    if (failed) break;
                }
            } else {
                if (els > -1) {
                    opindex = els;
                    for (; opindex < enf; opindex++) {
                        if (isop(stack[opindex])==OP_JMP) {
                            doop(isop(stack[opindex]));
                            jumped=true;
                            break;
                        }
                        doop(isop(stack[opindex]));
                        if (failed) break;
                    }
                }
            }
            if (!jumped)
                opindex = enf;
        } break;
        case OP_SET: {
            int setop = isop(stack[opindex+1]);
            if (setop == OP_MDX)
                memory[mds] = memory[mdx];
            else
                memory[mds] = atoi(stack[opindex+1]);
            opindex++;
        } break;
        case OP_ADD: {
            int addop = isop(stack[opindex+1]);
            if (addop == OP_MDX)
                memory[mds] += memory[mdx];
            else
                memory[mds] += atoi(stack[opindex+1]);
            opindex++;
        } break;
        case OP_ENF: {
            
        } break;
        case OP_PRN: {
            if (isop(stack[opindex+1]) == OP_MDX)
                printf("MDX: %d\n", memory[mdx]);
            else if (isop(stack[opindex+1]) == OP_MDS)
                printf("MDS: %d\n", memory[mds]);
            else
                printf("OUT: %s\n", stack[opindex+1]);
            opindex++;
        } break;
        case OP_ELS: {
        } break;
        case OP_MEM: {
            printf("MEM: ");
            for (int i = 0; i < MEM; i++) {
                printf("%d ", memory[i]);
            }
            printf("\n");
        } break;
        case OP_DEF: {
            opindex++;
        } break;
        case OP_JMP: {
            bool found=false;
            for (int i = 0; i < labelsize; i++) {
                if (!strcmp(stack[labels[i]], stack[opindex+1])) {
                    opindex = labels[i];
                    found=true;
                    break;
                }
            }
            if (!found) {
                printf("ERROR: NO LABEL (%s)\n", stack[opindex+1]);
                failed=true;
                break;
            }
        } break;
        case OP_IFL: {
            int els = -1;
            int enf = -1;
            for (int i = opindex; i < stacksize; i++) {
                int eop = isop(stack[i]);
                if (eop == OP_ENF) {
                    enf = i;
                    break;
                }
                if (eop == OP_ELS) {
                    els = i;
                }
            }
            if (enf == -1) {
                printf("ERROR: NO ENF\n");
                failed=true;
                break;
            }
            int m;
            bool jumped=false;
            if (isop(stack[opindex+1]) == OP_MDX) m = memory[mdx];
            else m = atoi(stack[opindex+1]);
            if (memory[mds] < atoi(stack[opindex+1])) {
                opindex+=2;
                int to = els > -1 ? els : enf;
                for (; opindex < to; opindex++) {
                    if (isop(stack[opindex])==OP_JMP) {
                        doop(isop(stack[opindex]));
                        jumped=true;
                        break;
                    }
                    doop(isop(stack[opindex]));
                    if (failed) break;
                }
            } else {
                if (els > -1) {
                    opindex = els;
                    for (; opindex < enf; opindex++) {
                        if (isop(stack[opindex])==OP_JMP) {
                            doop(isop(stack[opindex]));
                            jumped=true;
                            break;
                        }
                        doop(isop(stack[opindex]));
                        if (failed) break;
                    }
                }
            }
            if (!jumped)
                opindex = enf;
        } break;
        case OP_IFG: {
            int els = -1;
            int enf = -1;
            for (int i = opindex; i < stacksize; i++) {
                int eop = isop(stack[i]);
                if (eop == OP_ENF) {
                    enf = i;
                    break;
                }
                if (eop == OP_ELS) {
                    els = i;
                }
            }
            if (enf == -1) {
                printf("ERROR: NO ENF\n");
                failed=true;
                break;
            }
            int m;
            bool jumped=false;
            if (isop(stack[opindex+1]) == OP_MDX) m = memory[mdx];
            else m = atoi(stack[opindex+1]);
            if (memory[mds] > atoi(stack[opindex+1])) {
                opindex+=2;
                int to = els > -1 ? els : enf;
                for (; opindex < to; opindex++) {
                    if (isop(stack[opindex])==OP_JMP) {
                        doop(isop(stack[opindex]));
                        jumped=true;
                        break;
                    }
                    doop(isop(stack[opindex]));
                    if (failed) break;
                }
            } else {
                if (els > -1) {
                    opindex = els;
                    for (; opindex < enf; opindex++) {
                        if (isop(stack[opindex])==OP_JMP) {
                            doop(isop(stack[opindex]));
                            jumped=true;
                            break;
                        }
                        doop(isop(stack[opindex]));
                        if (failed) break;
                    }
                }
            }
            if (!jumped)
                opindex = enf;
        } break;
        case OP_MDX: {
            mdx = atoi(stack[opindex+1]);
            opindex++;
        } break;
        case OP_NEG: {
            memory[mds] = -memory[mds];
        } break;
        case OP_MUL: {
            int addop = isop(stack[opindex+1]);
            if (addop == OP_MDX)
                memory[mds] *= memory[mdx];
            else
                memory[mds] *= atoi(stack[opindex+1]);
            opindex++;
        } break;
        case OP_DIV: {
            int addop = isop(stack[opindex+1]);
            if (addop == OP_MDX)
                memory[mds] /= memory[mdx];
            else
                memory[mds] /= atoi(stack[opindex+1]);
            opindex++;
        } break;
        default: {
            printf("UNDEFINED OPERATION (%s)\n", stack[opindex]);
        } break;
    }
}

void stackinfo()
{
    printf("Stack has %d operations\n", stacksize);
    printf("[");
    for (int i = 0; i < stacksize; i++) {
        printf(" \"%s\"", stack[i]);
        if (i < stacksize-1) {
            printf(" ,");
        }
        printf(" ");
    }
    printf("]\n");
}
