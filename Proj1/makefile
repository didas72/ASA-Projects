CC=gcc
C_FLAGS=-DDEBUG -Wall -Wextra -Wno-unknown-pragmas -ggdb -lm
FINAL_C_FLAGS=-O3 -ansi -Wall -lm
VAL_FLAGS=--leak-check=full

BIN=build/bin
OUTBIN=$(BIN)/main
GENBIN=./supermarble


.PHONY: all build debug memleak gen_build clean setup


all: build
build: $(OUTBIN)
release: C_FLAGS=$(FINAL_C_FLAGS)
release: clean $(OUTBIN)
gen_build: $(GENBIN)

run: $(OUTBIN)
	./$(OUTBIN)

$(OUTBIN): src/main.c
	@mkdir -p $(@D)
	$(CC) $^ $(C_FLAGS) -o $@

debug: $(OUTBIN)
	gdb ./$(OUTBIN)

memleak: $(OUTBIN)
	valgrind $(VAL_FLAGS) $(OUTBIN)

$(GENBIN): src/aux/gen_supermarble.cpp
	g++ -std=c++11 -O3 -Wall $^ -o $(GENBIN)

clean:
	$(RM) -r $(BIN)

setup:
	sudo apt update
	sudo apt upgrade -y
	sudo apt install gcc gdb valgrind
