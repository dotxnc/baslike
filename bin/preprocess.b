(This is an example to show
the proprocessor finding defined
labels before you reach them.)

DEF START
MDS 0
IFE 10
    PRN IS_TEN
    JMP END
ELS
    PRN MDS
ENF
ADD 1
JMP START
DEF END
PRN ENDING
