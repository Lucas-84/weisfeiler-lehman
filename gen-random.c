// Generate two random graphs
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum { MAXN = 1000 };
int adj[MAXN][MAXN];

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: ./gen-random [nb_noeuds]\n");
    return 0;
  }

  srand(time(NULL));
  int N = atoi(argv[1]);
  assert(N <= MAXN);

  for (int k = 0; k < 2; ++k) {
    printf("%d\n", N);
    for (int i = 0; i < N; ++i)
      for (int j = i + 1; j < N; ++j)
        adj[i][j] = adj[j][i] = rand() % 2;

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
