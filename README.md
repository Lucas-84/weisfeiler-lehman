# Weisfeiler-Lehman heuristic for graph isomorphism

Compile the program with `make`, the resulting binary is `weisfeiler-lehman`. Clean the repository with `make clean`.

Usage: ./weisfeiler-lehman [w|b|d] file [-bound]

* w: Weisfeiler-Lehman (default)
* b: simple backtrack
* d: backtrack with degree
* file: input file in the specified format
* -bound: print the computed upper bound

Examples:
	
```
# Execute simple backtrack on a.in and print the computed upper bound
./weisfeiler-lehman b a.in -bound
# Execute Weisfeiler-Lehman on a.in
./weisfeiler-lehman a.in
```

You may want to compile the programs used to generate the examples (random graphs with Erdos-Renyi model):

* Random graphs: `gcc -Wall -Wextra -std=c99 gen-random.c -o gen-random`
* Random isomorphic graphs: `gcc -Wall -Wextra -std=c99 gen-random-perm.c -o gen-random-perm`

Test the program on all the files in `tests/` with `make test`. Tests are expected to represented isomorphic graphs iff. they end with *-perm.in.

http://www.di.ens.fr/algoL3/Projets/isomorphisme.pdf
