rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))
SRC = $(call rwildcard, src/, *.c) #$(wildcard src/*.cpp) $(wildcard src/engine/*.cpp)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))
CFLAGS = --std=c11 -Wno-incompatible-pointer-types -Wno-int-conversion -Isrc/
NAME = script
OUT =

LDFLAGS = -Wl,-allow-multiple-definition -static-libgcc
LIBS = 
CC = 
ifeq ($(OS),Windows_NT)
	OUT = $(NAME).exe
	CC += clang
	LIBS = 
else
	OUT = $(NAME)
	CC += gcc
	LIBS = 
endif

$(OUT): $(OBJ)
	@ test -d bin || mkdir bin
	$(CC) -o bin/$(OUT) $(CFLAGS) $(OBJ) $(LIBS) $(LDFLAGS)

obj/%.o: src/%.c
	@ test -d $(@D) || mkdir $(@D)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -r obj/*
	rm bin/$(OUT)
