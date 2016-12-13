BIN=weisfeiler-lehman
SOURCES=weisfeiler-lehman.c

compile:
	gcc -lm -O2 -Wall -Wextra -std=c99 $(SOURCES) -o $(BIN)

test: compile
	time ./test.sh

clean:
	rm -f $(BIN)

.PHONY: compile test

