// Generate two random isomorphic graphs (Erdos-Renyi)
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum { MAXN = 10000 };
int adj[MAXN][MAXN];
int p[MAXN];

void swap(int *p, int *q) { int t = *p; *p = *q; *q = t; }

int main(int argc, char *argv[]) {
	if (argc < 3) {
		fprintf(stderr, "Usage: ./gen-random [nb_noeuds] [P(arete entre u et v)]\n");
		return 0;
	}

	srand(time(NULL));
	int N = atoi(argv[1]);
	assert(N <= MAXN);
	int M = 0;
	double pr = strtod(argv[2], NULL);

	for (int i = 0; i < N; ++i)
		for (int j = i + 1; j < N; ++j) {
			int x = rand();
			if (x <= pr * RAND_MAX)
				adj[i][j] = adj[j][i] = 1;
			else
				adj[i][j] = adj[j][i] = 0;
			M += adj[i][j];
		}

	for (int i = 0; i < N; ++i) p[i] = i;

	for (int k = 0; k < 2; ++k) {
		printf("%d %d\n", N, M);
		for (int i = 0; i < N; ++i) {
			int d = 0;
			for (int j = 0; j < N; ++j) d += adj[p[i]][p[j]];
			printf("%d ", d);
			for (int j = 0; j < N; ++j) if (adj[p[i]][p[j]]) printf("%d ", j);
			puts("");
		}
		
		for (int i = N - 1; i > 0; --i) swap(&p[i], &p[rand() % (i + 1)]);
	}
	return 0;
}
