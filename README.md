# Preface
`BASLIKE` was created for a game I am/was working on called `^2`. The code is not pretty or written how an interpreter should be written but it works for what I need and that's all that matters.

# BASLIKE language specification
Current language spec is `0.5.1`

- [MDX and MDS](#mds-and-mdx)
- [Math ops](#math)
- [Debugging](#debugging)
- [Conditional statements](#conditional-if-statements)
- [Labels and looping](#labels-and-looping)
- [Fun](#fun)
- [Comments](#comments)

### MDS and MDX
___
The way variables are stored use the `MDS` and `MDX` registers. `MDS` is the write register and `MDX` is the read register. Together they're used as the backbone of all logic in `BASLIKE`. "Memory" in baslike is represented as an array of 8 integer values and accessed using the read and write registers.

Let's look at a code example
```
MDS 0
SET 5
MDS 1
SET 10
MDS 0
MDX 1
ADD MDX
```
The resulting memory layout should be `[15, 10, 0, 0, 0, 0, 0, 0]`.
Let's walk through this:

`MDS 0` sets the write register to location 0 in memory. `SET 5` sets the value at the write register to `5`, Next we set the write register to location 1 (`MDS 1`) and set that value to `10`. The memory layout at this point is `[5, 10, 0, 0, 0, 0, 0, 0]`. We then set the write register to 0 and the *read* register to 1 and `ADD MDX` which adds the value pointed to by the read register to the value pointed to by the write register.

Hopefully that all makes at least *some* sense, we'll cover basic math next.

### Math
___
~~Math in `BASLIKE` is done using just three ops and those are: `SET`, `ADD`, and `NEG`. You'll note that there is no multiplication or division here, all of that has to be done manually using loops which we'll cover at a later point.~~

As of `BASLIKE` 0.4 you have 5 math ops: `SET`, `ADD`, `MUL`, `DIV`, `NEG`. See the bin folder for examples.

**NOTE**: All memory is stored as signed 32 bit integers; there are no floats or doubles and as such all non whole numbers are floored.

All of these operations can either use a value or the read register `MDX` apart from `NEG` which only does operations on the write register `MDS`.

Let's look at some code:
```
MDS 0
SET 10
MDS 1
SET 25
NEG
MDS 0
MDX 1
ADD MDX
```
The memory layout after `NEG` should be `[10, -25, 0, 0, 0, 0, 0, 0]`. In `BASLIKE` the only way to subtract is to add negative numbers using the `ADD` operation. Memory layout in the end should look like `[-15, -25, 0, 0, 0, 0, 0, 0]`.

### Debugging
___
Throughout your programming you might run into some weird problems and that is where the `PRN` and `MEM` operations come in handy. `PRN` prints out a single word or alternatively you can specify to output `MDS` or `MDX` to stdout. `MEM` outputs the current memory layout.

Let's take the simple math example and add some debugging to it to make sure everything works properly:
```
PRN START
MDS 0
SET 10
MDS 1
SET 25
NEG
MEM
MDS 0
MDX 1
ADD MDX
MEM
PRN END
```
The expected output of running this script is:
```
OUT: START
MEM: 10, -25, 0, 0, 0, 0, 0, 0
MEM: -15, -20, 0, 0, 0, 0, 0, 0
OUT: END
```

### Conditional IF statements
___
Conditional statements are create using three different `IFX` registers and are: `IFE`, `IFL`, and `IFG`. These stand for "If equal", "If less than", and "If greater than". There's a fourth register called `ELS` and that's used for the `else` condition. All of these take either an integer as an argument or `MDX` as usual. All if statements need to be closed with an `ENF` operation.

Some example code again:
```
MDS 0
SET 10
IFE 5
    PRN IS_FIVE
ELS
    PRN NOT_FILE
    IFE 10
        PRN IS_TEN
    ENF
ENF
```
I'm going to assume by now you can read the code fairly easily so I won't go into much detail. This code is fairly straightforward.

### Labels and looping
___
Labels are used by two operations: `DEF` and `JMP`. `DEF NAME` defines a label with name `NAME` which can then be jumped to using `JMP NAME`.

~~**NOTE**: In version 0.3 of the `BASLIKE` specification you can't jump to a label that wasn't created yet.~~
This was corrected in 0.3.1 with the inclusion of the preprocessor

Labels make loops fairly simple, you just need a place in memory to store the current index of the loop and a label. Here's an example of a ranged for loop:
```
DEF LOOP
MDS 0
IFL 5
    ADD 1
    PRN MDS
    JMP LOOP
ENF
PRN END
```
The expected output is something along the lines of
```
MDS: 1
MDS: 2
MDS: 3
MDS: 4
MDS: 5
OUT: END
```

### Fun
___
Just for fun, here is an implementation of the fibonacci sequence in `BASLIKE`
```
MDS 2
SET 1
DEF FIB
MDS 0
IFL 10
    ADD 1
    MDS 1
    PRN MDS
    MDS 3
    MDX 1
    SET MDX
    MDX 2
    ADD MDX
    MDS 1
    MDX 2
    SET MDX
    MDS 2
    MDX 3
    SET MDX
    JMP FIB
ENF
```

### Comments
___
Commenting code can be important so I've implemented a very simplistic way of documenting code. ~~If a word contains any lowercase letter it gets ignored by the interpreter.~~
**NOTE**: Comments were changed in 0.5 to the following.
```
(This is a comment)
MDS 0
ADD 5
```
This works fine and the resulting output is just as you'd expect.
```
(
THIS is still a comment
)
MDS 0
ADD 5
```
~~This will cause a warning of `NON OPERATION (THIS)`.~~ See previous note.
