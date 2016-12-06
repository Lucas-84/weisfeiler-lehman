# Weisfeiler-Lehman heuristic for graph isomorphism


Compile the program with `make`, the resulting binary is `a`.

Test the program on all the files in `tests/` with `make test`.

You may want to compile the programs used to generate the examples :

* Random graphs: `gcc -Wall -Wextra -std=c99 gen-random.c -o gen-random`
* Random isomorphic graphs: `gcc -Wall -Wextra -std=c99 gen-random-perm.c -o gen-random-perm`

http://www.di.ens.fr/algoL3/Projets/isomorphisme.pdf
