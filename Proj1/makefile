CC=gcc
C_FLAGS=-ansi -Wall -Wextra -Wno-unknown-pragmas -ggdb -lm# -DDEBUG 
FINAL_C_FLAGS=-O3 -ansi -Wall -lm
VAL_FLAGS=--leak-check=full

BIN=build/bin
TEST=tests
OUTBIN=$(BIN)/main
GENBIN=./supermarble

TESTS=$(wildcard $(TEST)/*.in)
RESULTS=$(patsubst %.in,%.out,$(TESTS))

.PHONY: all build debug memleak gen_build clean setup test


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

test: #clean release
	@mkdir -p test_res
	echo "" > test_res/test_effective_out
	for testfile in $(TESTS) ; do \
		$(OUTBIN) < $$testfile >> test_res/test_effective_out; \
	done
	echo "" > test_res/test_expected_out
	for resultfile in $(RESULTS) ; do \
		cat $$resultfile >> test_res/test_expected_out; \
	done
	echo "" > test_res/test_order
	for testfile in $(TESTS) ; do \
		echo $$testfile >> test_res/test_order; \
	done
	diff test_res/test_effective_out test_res/test_expected_out > test_res/00_diff

clean-test:
	$(RM) -r test_res

