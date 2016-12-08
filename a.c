#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct graph {
  int **adj;
  int *deg;
  int nb_nodes;
  int nb_edges;
};

struct partition {
  int **parts;
  int **adjl;
  int *degree;
  int *node_part;
  int *part_size; 
  int idx_min;
  int nb_parts;
};

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

/* Return 1 if a is a g1-to-g2-isomorphism, 0 otherwise */
/* a must be a bijection of [1, n] with n = g1->nb_nodes = g2->nb_nodes */ 
int is_isomorphism_partial(const struct graph *g1, const struct graph *g2, const int *a, const int *e) {
  int n = g1->nb_nodes;
  for (int u = 0; u < n; ++u)
    if (e[u])
      for (int v = 0; v < n; ++v)
        if (e[v] && g1->adj[u][v] != g2->adj[a[u]][a[v]])
          return 0;
  return 1;
}

int is_isomorphism(const struct graph *g1, const struct graph *g2, const int *a) {
  int n = g1->nb_nodes;
  int *e = malloc_wrapper(n * sizeof *e);
  for (int i = 0; i < n; ++i)
    e[i] = 1;
  int ret = is_isomorphism_partial(g1, g2, a, e);  
  free(e);
  return ret;
}

static struct partition *pcmp;

int compare_lists(const int *p, int n, const int *q, int m) {
  int i = 0, j = 0;
  while (i < n && j < m) {
    int a = pcmp->node_part[p[i]], b = pcmp->node_part[q[i]];
    if (a != b)
      return a < b ? -1 : +1;
    i++; j++;
  }
  if (i == n && j == m)
    return 0;
  return i < n ? -1 : +1;
}

int compare_nodes(const void *p, const void *q) {
  int u = *(const int *)p, v = *(const int *)q;
  return compare_lists(pcmp->adjl[u], pcmp->degree[u], pcmp->adjl[v], pcmp->degree[v]);
}

int compare_node_parts(const void *p, const void *q) {
  int u = *(const int *)p, v = *(const int *)q;
  int pu = pcmp->node_part[u], pv = pcmp->node_part[v];
  if (pu == pv) return 0;
  if (pu < pv) return -1;
  return 1;
}

void stable_partition(const struct graph *g, struct partition *p) {
  int n = g->nb_nodes;
  int *succ_node_part = malloc_wrapper(n * sizeof *succ_node_part);
  int has_changed = 0;

  for (int i = 0; i < n; ++i)
    succ_node_part[i] = p->node_part[i];
 
  do {
    int prev_nb_parts = p->nb_parts;
    has_changed = 0;
    pcmp = p;

    for (int u = 0; u < n; ++u)
      qsort(p->adjl[u], p->degree[u], sizeof(int), compare_node_parts);

    for (int i = 0; i < prev_nb_parts; ++i) {
      qsort(p->parts[i], p->part_size[i], sizeof(int), compare_nodes);
      
      int last = p->part_size[i] - 1;

      for (int j = p->part_size[i] - 2; j >= 0; --j) {
        int u = p->parts[i][j], v = p->parts[i][j + 1];

        if (compare_lists(p->adjl[u], p->degree[u], p->adjl[v], p->degree[v]) != 0) {
          for (int k = j + 1; k <= last; ++k) {
            p->parts[p->nb_parts][k - (j + 1)] = p->parts[i][k];
            succ_node_part[p->parts[i][k]] = p->nb_parts;
          }

          p->part_size[p->nb_parts] += last - j;
          p->nb_parts++;
          last = j;
          has_changed = 1;
        }
      }

      p->part_size[i] = last + 1;
    }

    for (int u = 0; u < n; ++u)
      p->node_part[u] = succ_node_part[u];
  } while (has_changed);

  free(succ_node_part);
} 

int weisfeiler_lehman(const struct graph *g1, const struct graph *g2, int *a) {
  int n = g1->nb_nodes;
  struct partition p;
  p.part_size = calloc_wrapper(n, sizeof *p.part_size);
  p.degree = malloc_wrapper(n * sizeof *p.degree);
  p.node_part = calloc_wrapper(n, sizeof *p.node_part);
  p.parts = malloc_wrapper(n * sizeof *p.parts);
  for (int u = 0; u < n; ++u) p.parts[u] = malloc_wrapper(n * sizeof *p.parts[u]);
  p.adjl = malloc_wrapper(n * sizeof *p.adjl);
  for (int u = 0; u < n; ++u) p.adjl[u] = malloc_wrapper(n * sizeof *p.adjl[u]);

  p.nb_parts = 1;
  p.part_size[0] = n;
  for (int u = 0; u < n; ++u)
    p.parts[0][u] = u;

  for (int u = 0; u < n; ++u) {
    p.degree[u] = 0;
    for (int v = 0; v < n; ++v)
      if (g1->adj[u][v])
        p.adjl[u][p.degree[u]++] = v;
  }

  stable_partition(g1, &p);
  
  for (int i = 0; i < p.nb_parts; ++i) {
    printf("Part %d %d ", i, p.part_size[i]);
    for (int j = 0; j < p.part_size[i]; ++j) printf("%d ", p.parts[i][j]);
    puts("");
  }

  for (int u = 0; u < n; ++u) free(p.adjl[u]);
  free(p.adjl);
  for (int u = 0; u < n; ++u) free(p.parts[u]);
  free(p.parts);
  free(p.node_part);
  free(p.part_size);
  free(p.degree);
  return -1;
}

/* Return 1 if g1 and g2 are isomorphic, 0 otherwise */
/* In the first case, a will contain the isomorphism at the end of simple_backtrack(g1, g2, a, 0) */
/* Algorithm: simple backtrack */
int backtrack_simple(const struct graph *g1, const struct graph *g2, int *a, int u) {
  if (g1->nb_nodes != g2->nb_nodes) return 0;
  int n = g1->nb_nodes;
  if (u == n) return 1;
  for (int v = 0; v < n; ++v) {
    int possible = 1;
    for (int i = 0; i < u; ++i)
      if (a[i] == v || g1->adj[u][i] != g2->adj[v][a[i]] || g1->adj[i][u] != g2->adj[a[i]][v]) {
        possible = 0;
        break;
      }
    if (possible) {
      a[u] = v;
      if (backtrack_simple(g1, g2, a, u + 1))
        return 1;
    }
  }
  return 0;
}

/* Same as above */
/* Algorithm: backtrack with degree partition */
int backtrack_degree(const struct graph *g1, const struct graph *g2, int *a, int u) {
  if (g1->nb_nodes != g2->nb_nodes) return 0;
  int n = g1->nb_nodes;
  if (u == n) return 1;
  for (int v = 0; v < n; ++v) {
    if (g1->deg[u] != g2->deg[v]) continue;
    int possible = 1;
    for (int i = 0; i < u; ++i)
      if (a[i] == v || g1->adj[u][i] != g2->adj[v][a[i]] || g1->adj[i][u] != g2->adj[a[i]][v]) {
        possible = 0;
        break;
      }
    if (possible) {
      a[u] = v;
      if (backtrack_degree(g1, g2, a, u + 1))
        return 1;
    }
  }
  return 0;
}


struct graph *read_graph(void) {
  struct graph *g = malloc_wrapper(sizeof *g);
  scanf("%d%d", &g->nb_nodes, &g->nb_edges);
  g->adj = malloc_wrapper(g->nb_nodes * sizeof *g->adj);
  g->deg = malloc_wrapper(g->nb_nodes * sizeof *g->deg);
  for (int u = 0; u < g->nb_nodes; ++u)
    g->adj[u] = calloc_wrapper(g->nb_nodes, sizeof *g->adj[u]);
  for (int u = 0; u < g->nb_nodes; ++u) {
    scanf("%d", &g->deg[u]);
    for (int i = 0; i < g->deg[u]; ++i) {
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
  //struct graph *g2 = read_graph();
  weisfeiler_lehman(g1, NULL, NULL);
  /*
  int *a = calloc_wrapper(g1->nb_nodes, sizeof *a);

  //if (!backtrack_simple(g1, g2, a, 0)) {
  if (!backtrack_degree(g1, g2, a, 0)) {
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
  */
  return EXIT_SUCCESS;
}
