BIN=weisfeiler-lehman
SOURCES=weisfeiler-lehman.c

compile:
	gcc -O3 -Wall -Wextra -std=c99 $(SOURCES) -o $(BIN) -lm

test: compile
	time ./test.sh

clean:
	rm -f $(BIN)

.PHONY: compile test
