BIN=a

compile:
	gcc -Wall -Wextra -std=c99 a.c -o $(BIN)

test: compile
	time ./test.sh

clean:
	rm -f $(BIN)

.PHONY: compile test

