#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct graph {
  int **adj;
  int nb_nodes;
};

/* Return 1 if a is a g1-to-g2-isomorphism, 0 otherwise */
/* a must be a bijection of [1, n] with n = g1->nb_nodes = g2->nb_nodes */ 
int is_isomorphism(const struct graph *g1, const struct graph *g2, const int *a) {
  int n = g1->nb_nodes;
  for (int u = 0; u < n; ++u)
    for (int v = 0; v < n; ++v)
      if (g1->adj[u][v] != g2->adj[a[u]][a[v]])
        return 0;
  return 1;
}

/* Return 1 if g1 and g2 are isomorphic, 0 otherwise */
/* In the first case, a will contain the isomorphism at the end of simple_backtrack(g1, g2, a, 0) */
/* Algorithm: simple backtrack */
int simple_backtrack(const struct graph *g1, const struct graph *g2, int *a, int u) {
  if (g1->nb_nodes != g2->nb_nodes) return 0;
  int n = g1->nb_nodes;
  if (u == n) return 1;
  for (int v = 0; v < n; ++v) {
    int possible = 1;
    for (int i = 0; i < u; ++i) {
      if (a[i] == v || g1->adj[u][i] != g2->adj[v][a[i]] || g1->adj[i][u] != g2->adj[a[i]][v]) {
        possible = 0;
        break;
      }
    }
    if (possible) {
      a[u] = v;
      if (simple_backtrack(g1, g2, a, u + 1))
        return 1;
    }
  }
  return 0;
}

void *malloc_wrapper(size_t sz) {
  void *p = malloc(sz);
  if (p == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  return p;
}

void *calloc_wrapper(size_t nmemb, size_t sz) {
  void *p = calloc(nmemb, sz);
  if (p == NULL) {
    perror("calloc");
    exit(EXIT_FAILURE);
  }
  return p;
}

struct graph *read_graph(void) {
  struct graph *g = malloc_wrapper(sizeof *g);
  scanf("%d", &g->nb_nodes);
  g->adj = malloc_wrapper(g->nb_nodes * sizeof *g->adj);
  for (int u = 0; u < g->nb_nodes; ++u)
    g->adj[u] = calloc_wrapper(g->nb_nodes, sizeof *g->adj[u]);
  for (int u = 0; u < g->nb_nodes; ++u) {
    int nb_neighbours;
    scanf("%d", &nb_neighbours);
    for (int i = 0; i < nb_neighbours; ++i) {
      int v;
      scanf("%d", &v);
      g->adj[u][v] = 1;
    }
  }
  return g;
}
  
void free_graph(struct graph *g) {
  for (int i = 0; i < g->nb_nodes; ++i)
    free(g->adj[i]);
  free(g->adj);
  free(g);
}

int main(void) {
  struct graph *g1 = read_graph(); 
  struct graph *g2 = read_graph();
  int *a = calloc_wrapper(g1->nb_nodes, sizeof *a);
  if (!simple_backtrack(g1, g2, a, 0)) {
    puts("non");
  } else {
    puts("oui");
    assert(is_isomorphism(g1, g2, a));
    for (int i = 0; i < g1->nb_nodes; ++i)
      printf("%d ", a[i]);
    puts("");
  }
  free_graph(g1);
  free_graph(g2);
  return EXIT_SUCCESS;
}
