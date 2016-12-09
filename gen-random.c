// Generate two random graphs (Erdos-Renyi)
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum { MAXN = 10000 };
int adj[MAXN][MAXN];

int main(int argc, char *argv[]) {
	if (argc < 3) {
		fprintf(stderr, "Usage: ./gen-random [nb_noeuds] [P(arete entre u et v)]\n");
		return 0;
	}

	srand(time(NULL));
	int N = atoi(argv[1]);
	assert(N <= MAXN);
	double p = strtod(argv[2], NULL);

	for (int k = 0; k < 2; ++k) {
		int M = 0;

		for (int i = 0; i < N; ++i)
			for (int j = i + 1; j < N; ++j) {
				int x = rand();
				if (x <= RAND_MAX * p)
					adj[i][j] = adj[j][i] = 1;
				M += adj[i][j];
			}
		
		printf("%d %d\n", N, M);

		for (int i = 0; i < N; ++i) {
			int d = 0;
			for (int j = 0; j < N; ++j) d += adj[i][j];
			printf("%d ", d);
			for (int j = 0; j < N; ++j) if (adj[i][j]) printf("%d ", j);
			puts("");
		}
	}
	return 0;
}
