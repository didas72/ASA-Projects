CC=g++
C_FLAGS=-Wall -Wextra -Wno-unknown-pragmas -ggdb -lm -DDEBUG
FINAL_C_FLAGS=-std=c++11 -O3 -Wall -lm
VAL_FLAGS=--leak-check=full -s

BIN=build/bin
TEST=tests
OUTBIN=$(BIN)/main

TESTS=$(wildcard $(TEST)/*.in)
RESULTS=$(patsubst %.in,%.out,$(TESTS))

.PHONY: all build debug memleak clean setup test


all: build
build: $(OUTBIN)
release: C_FLAGS=$(FINAL_C_FLAGS)
release: clean $(OUTBIN)

run: $(OUTBIN)
	./$(OUTBIN)

$(OUTBIN): src/main.cpp
	@mkdir -p $(@D)
	$(CC) $^ $(C_FLAGS) -o $@

debug: $(OUTBIN)
	gdb ./$(OUTBIN)

memleak: $(OUTBIN)
	valgrind $(VAL_FLAGS) $(OUTBIN)

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

