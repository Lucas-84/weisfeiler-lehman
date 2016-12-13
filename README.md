# Weisfeiler-Lehman heuristic for graph isomorphism

`weisfeiler-lehman` is an implementation of the Weisfeiler-Lehman algorithm using C99.

## Basic usage

Compile the program with `make`, the resulting binary is `weisfeiler-lehman`. Clean the repository with `make clean`.

Usage: `./weisfeiler-lehman [w|b|d] file [-bound]`

* `w`: Weisfeiler-Lehman (default)
* `b`: simple backtrack
* `d`: backtrack with degree
* `file`: input file in the specified format in http://www.di.ens.fr/algoL3/Projets/projet_base.html
* `-bound`: print the computed upper bound

Examples:
	
```
# Execute simple backtrack on a.in and print the computed upper bound
./weisfeiler-lehman b a.in -bound
# Execute Weisfeiler-Lehman on a.in
./weisfeiler-lehman a.in
```

## Some basic tests

You may want to compile the programs used to generate the examples (random graphs with Erdos-Renyi model):

* Random graphs: `gcc -Wall -Wextra -std=c99 gen-random.c -o gen-random`
* Random isomorphic graphs: `gcc -Wall -Wextra -std=c99 gen-random-perm.c -o gen-random-perm`

Test the program on all the files in `tests/` with `make test`. Tests are expected to represented isomorphic graphs iff. they end with `*-perm.in`.

## Directory contents

* `Makefile`
* `README.md`
* `gen-random.c`
* `gen-random-perm.c`
* `checker.cpp` (checker for stable partitioning)
* `notes` (some remainder to generate tests)
* `weisfeiler-lehman.c` (main program source)
* `test.sh` (script for testing)

## Miscellaneous

There is no `*_bound` function for the functions in the main part of the algorithm (`test_all_alone`, `test_one`, `weisfeiler_lehman_from`, `weisfeiler_lehman`, `backtrack_simple`, `backtrack_degree`, `find_stable_partition`), but rather a parameter `bound`. The goal is to achieve more precise time estimations.

The algorithm is implemented in the way it is described in the problem statement. In order to get a stable partitioning from an initial partitioning, we mainly sort the adjacency lists according to the parts to which the neighbours of the node belong.

## Reference 

http://www.di.ens.fr/algoL3/Projets/isomorphisme.pdf
