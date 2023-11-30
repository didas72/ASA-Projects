CC=g++
C_FLAGS=-DDEBUG -Wall -Wextra -Wno-unknown-pragmas -std=c++11 -ggdb -lm
FINAL_C_FLAGS=-std=c++11 -O3 -Wall -lm
VAL_FLAGS=--leak-check=full

SRC=src
OBJ=build/obj
BIN=build/bin

OUTBIN=$(BIN)/main

SRCS=$(wildcard $(SRC)/*.cpp)
OBJS=$(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRCS))


.PHONY: all build debug memleak clean loc setup


all: build
build: $(OUTBIN)

run: $(OUTBIN)
	./$(OUTBIN)

$(OUTBIN): $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(C_FLAGS) $(OBJS) -o $@

$(OBJ)/%.o: $(SRC)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(C_FLAGS) -c $< -o $@


debug: $(OUTBIN)
	gdb ./$(OUTBIN)

memleak: $(OUTBIN)
	valgrind $(VAL_FLAGS) $(OUTBIN)


clean:
	$(RM) -r $(OBJ) $(BIN)

loc:
	scc -s lines --no-cocomo --no-gitignore -w --size-unit binary --exclude-ext md,makefile,json

setup:
	sudo apt update
	sudo apt upgrade -y
	sudo apt install g++ gdb valgrind
