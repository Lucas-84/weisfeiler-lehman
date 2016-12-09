BIN=a

compile:
	gcc -O2 -Wall -Wextra -std=c99 a.c -o $(BIN)

test: compile
	time ./test.sh

clean:
	rm -f $(BIN)

.PHONY: compile test

