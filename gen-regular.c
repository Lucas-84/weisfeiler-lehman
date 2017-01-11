#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum { MAXN = 10000, MAXK = 1000 };
int omega[MAXN * MAXK];
int adj[MAXN][MAXN];
int degree[MAXN];
int seen[MAXN * MAXK]; 

void swap(int *p, int *q) { int t = *p; *p = *q; *q = t; }

int is_suitable(int k, int omega_size) {
	for (int i = 0; i < omega_size; ++i)
		for (int j = 0; j < omega_size; ++j) {
			int u = omega[i], v = omega[j];
			if (u / k != v / k && !adj[u / k][v / k] && !seen[u] && !seen[v]) 
				return 1;
		}
	return 0;
}

int generate_graph(int n, int k) {
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j)
			adj[i][j] = 0;
		degree[i] = 0;
	}

	int omega_size = k * n;
	for (int i = 0; i < n * k; ++i) {
		omega[i] = i;
		seen[i] = 0;
	}

	while (is_suitable(k, omega_size)) {
		int i = rand() % omega_size, j = rand() % omega_size;
		int u = omega[i], v = omega[j];
		if (u / k != v / k && !adj[u / k][v / k] && !seen[u] && !seen[v]) {
			degree[u / k]++;
			degree[v / k]++;
			seen[u] = seen[v] = 1;
			adj[u / k][v / k] = adj[v / k][u / k] = 1;
			swap(&omega[i], &omega[--omega_size]);
			swap(&omega[j], &omega[--omega_size]);
		}
	}

	for (int i = 0; i < n; ++i)
		if (degree[i] != k)
			return 0;

	return 1;
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Usage: ./gen-regular n k\n");
		return EXIT_FAILURE;
	}

	int n = atoi(argv[1]);		
	int k = atoi(argv[2]);

	if (n > MAXN) {
		fprintf(stderr, "Maxn = %d\n", MAXN);
		return EXIT_FAILURE;
	}
	
	if (k > MAXK || k > n) {
		fprintf(stderr, "k is too big (Maxk = %d; n = %d)\n", MAXK, n);
		return EXIT_FAILURE;
	}

	if (n * k % 2 != 0) {
		fprintf(stderr, "n*k must be even\n");
		return EXIT_FAILURE;
	}
		
	srand(time(NULL));
	for (int it = 0; it < 2; ++it) {
		while (!generate_graph(n, k))
			;
		
		printf("%d %d\n", n, n * k / 2);
		for (int i = 0; i < n; ++i) {
			printf("%d ", degree[i]);
			for (int j = 0; j < n; ++j)
				if (adj[i][j])
					printf("%d ", j);
			puts("");
		}
	}
	return 0;
}

