#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef long long ll;
int max(int a, int b) { return a > b ? a : b; }
int min(int a, int b) { return a < b ? a : b; }

/* Graph data structure */
/* Representation with an adjacency matrix */
struct graph {
	/* adj[u][v] = 1 iff. (u, v) in E */
	int **adj;
	/* deg[u] := |{(u, v) in E, v in V}| */
	int *deg;
	int nb_nodes, nb_edges;
};

/* Representation of a partition */
struct partition {
	/* parts[i] := list of the nodes in the i-th part */
	int **parts;
	/* Adjacency list */
	int **adjl;
	/* degree[u] is the degree of u in G */
	int *degree;
	/* node_part[u] = i iff. u is in the i-th part */
	int *node_part;
	/* part_size[i] := size of the ith part */
	int *part_size; 
	/* Number of parts */
	int nb_parts;
};

void *malloc_wrapper(size_t sz);
ll malloc_wrapper_bound(void);
void *calloc_wrapper(size_t nmemb, size_t sz);
ll calloc_wrapper_bound(void);
int is_isomorphism_partial(const struct graph *g1, const struct graph *g2,
			   const int *a, const int *e);
ll is_isomorphism_partial_bound(int n); 
int is_isomorphism(const struct graph *g1, const struct graph *g2, const int *a);
ll is_isomorphism_bound(int n); 
int compare_lists(const struct partition *p1, const int *p, int n,
		  const struct partition *p2, const int *q, int m);
ll compare_lists_bound(int deg1, int deg2);
int compare_nodes(const void *p, const void *q);
ll compare_nodes_bound(int degmax);
int compare_node_parts(const void *p, const void *q);
ll compare_node_parts_bound(void);
ll sort_adjacency_lists_bound(int n, struct partition *p);
void add_new_part(struct partition *p, int i, int start, int end, int *succ_node_part);
ll add_new_part_bound(int start, int end);
int stabilize_part(struct partition *p, int *succ_node_part, int i);
int stabilize_part(struct partition *p, int *succ_node_part, int i);
ll stabilize_part_bound(const struct partition *p, int i, int degmax);
void find_stable_partition(const struct graph *g, struct partition *p, ll *bound);
int are_incompatible(const struct partition *p1, const struct partition *p2);
ll are_incompatible_bound(const struct partition *p1, const struct partition *p2);
void isolate(struct partition *p, int ipart, int i);
ll isolate_bound(const struct partition *p, int ipart, int i);
int smallest_part(const struct partition *p, const int *e);
ll smallest_part_bound(const struct partition *p);
void copy_partition(int n, struct partition *p, const struct partition *old);
int copy_partition_bound(int n);
void free_partition(int n, struct partition *p);
ll free_partition_bound(int n);
int test_all_alone(struct partition *p1, struct partition *p2, const struct graph *g1,
		   const struct graph *g2, int *a, int *e, int k, ll *bound);
int test_one(int imin, struct partition *p1, struct partition *p2, const struct graph *g1,
	     const struct graph *g2, int *a, int *e, int k, ll *bound);
int weisfeiler_lehman_from(struct partition *p1, struct partition *p2, const struct graph *g1,
 			   const struct graph *g2, int *a, int *e, int k, ll *bound);
struct partition create_partition(const struct graph *g);
ll create_partition_bound(int n);
ll free_partition_bound(int n);
int weisfeiler_lehman(const struct graph *g1, const struct graph *g2, int *a, ll *bound);
int backtrack_simple(const struct graph *g1, const struct graph *g2, int *a, int u, ll *bound);
int backtrack_degree(const struct graph *g1, const struct graph *g2, int *a, int u, ll *bound);
struct graph *read_graph(void);
void free_graph(struct graph *g);
void usage(const char *name);

/* Some wrappers for handling errors in dynamic memory management */
void *malloc_wrapper(size_t sz) {
	void *p = malloc(sz);

	if (p == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	return p;
}

ll malloc_wrapper_bound(void) {
	return 4;
}

void *calloc_wrapper(size_t nmemb, size_t sz) {
	void *p = calloc(nmemb, sz);

	if (p == NULL) {
		perror("calloc");
		exit(EXIT_FAILURE);
	}

	return p;
}

ll calloc_wrapper_bound(void) {
	return 4;
}

/* assert(g1->nb_nodes == g2->nb_nodes) */
/*
 * Returns 1 if a : [1, n] -> [1, n] is a bijection from the set represented
 * by e to its image
 */
int is_isomorphism_partial(const struct graph *g1, const struct graph *g2,
			   const int *a, const int *e) {
	int n = g1->nb_nodes;
	for (int u = 0; u < n; ++u)
		if (e[u])
			for (int v = 0; v < n; ++v)
				if (e[v] && g1->adj[u][v] != g2->adj[a[u]][a[v]])
					return 0;
	return 1;
}

ll is_isomorphism_partial_bound(int n) {
	return 1 + n * (1 + n * 2) + 1;
}

/*
 * Return 1 if a is a g1-to-g2-isomorphism, 0 otherwise
 * a must be a bijection of [1, n] with n = g1->nb_nodes = g2->nb_nodes
 */ 
int is_isomorphism(const struct graph *g1, const struct graph *g2, const int *a) {
	int n = g1->nb_nodes;
	int *e = malloc_wrapper(n * sizeof *e);
	for (int i = 0; i < n; ++i)
		e[i] = 1;
	int ret = is_isomorphism_partial(g1, g2, a, e);	
	free(e);
	return ret;
}

ll is_isomorphism_bound(int n) {
	return 1 + malloc_wrapper_bound() + n + is_isomorphism_partial_bound(n) + 2;	
}

/*
 * Return -1, 0 or +1 according to the relative order of p and q
 *
 * p and q are supposed to be adjacency list of two nodes in (g1, p1) and
 * (g2, p2) respectively
 * Lists are classified lexicographically by the index of the parts of the list elements
 */
int compare_lists(const struct partition *p1, const int *p, int n,
		  const struct partition *p2, const int *q, int m) {
	int i = 0, j = 0;
	while (i < n && j < m) {
		int a = p1->node_part[p[i]], b = p2->node_part[q[i]];
		if (a != b)
			return a < b ? -1 : +1;
		i++; j++;
	}
	if (i == n && j == m)
		return 0;
	return i < n ? -1 : +1;
}

ll compare_lists_bound(int deg1, int deg2) {
	return 2 + max(deg1, deg2) * 5 + 3;
}

/* Global variable used in compared_nodes */
/* It is the partition refered by the sorting algorithm */
static struct partition *pcmp;

/*
 * Return -1, 0 or +1 if u should go before v in the "natural order" described
 * above, induced by lexicographic order on the adjacency list
 */
int compare_nodes(const void *p, const void *q) {
	int u = *(const int *)p, v = *(const int *)q;
	return compare_lists(pcmp, pcmp->adjl[u], pcmp->degree[u],
			     pcmp, pcmp->adjl[v], pcmp->degree[v]);
}

ll compare_nodes_bound(int degmax) {
	return 2 + compare_lists_bound(degmax, degmax);
}

/* Return -1, 0 or +1 if u should go before v in the "natural order" described above */
int compare_node_parts(const void *p, const void *q) {
	int u = *(const int *)p, v = *(const int *)q;
	int pu = pcmp->node_part[u], pv = pcmp->node_part[v];

	if (pu == pv)
		return 0;

	return pu < pv ? -1 : +1;
}

ll compare_node_parts_bound(void) {
	return 5;
}

/*
void insertion_sort(void *vp, int nmemb, int size, int (*compare)(const void *, const void *)) {
	char *buf = malloc_wrapper(size);
	char *p = vp;
	for (int i = 0; i < nmemb; ++i) {
		char *q = p + i * size;
		while (q > p && compare(q - size, q) > 0) {
			memcpy(buf, q - size, size);
			memcpy(q - size, q, size);
			memcpy(q, buf, size);
			q -= size;
		}
	}
	free(buf);
}
*/

void sort_adjacency_lists(int n, struct partition *p) {
	pcmp = p;

	for (int u = 0; u < n; ++u)
		qsort(p->adjl[u], p->degree[u], sizeof(int), compare_node_parts);
}

ll sort_adjacency_lists_bound(int n, struct partition *p) {
	ll ans = 0;
	for (int u = 0; u < n; ++u)
		ans += p->degree[u] * (int)log(p->degree[u]) * compare_node_parts_bound();
	return ans;
}

void add_new_part(struct partition *p, int i, int start, int end, int *succ_node_part) {
	for (int k = start; k <= end; ++k) {
		p->parts[p->nb_parts][k - start] = p->parts[i][k];
		succ_node_part[p->parts[i][k]] = p->nb_parts;
	}

	p->part_size[p->nb_parts] += end - start + 1;
	p->nb_parts++;
}

ll add_new_part_bound(int start, int end) {
	return 2 * (end - start + 1) + 2;	
}

int stabilize_part(struct partition *p, int *succ_node_part, int i) {
	int has_changed = 0;
	qsort(p->parts[i], p->part_size[i], sizeof(int), compare_nodes);
			
	int last = p->part_size[i] - 1;

	for (int j = p->part_size[i] - 2; j >= 0; --j) {
		int u = p->parts[i][j], v = p->parts[i][j + 1];

		if (compare_lists(p, p->adjl[u], p->degree[u], p, p->adjl[v], p->degree[v]) != 0) {
			add_new_part(p, i, j + 1, last, succ_node_part);
			has_changed = 1;
			last = j;
		}
	}

	p->part_size[i] = last + 1;
	return has_changed;
}

ll stabilize_part_bound(const struct partition *p, int i, int degmax) {
	ll ans = p->part_size[i] * (int)log(p->part_size[i]) * compare_nodes_bound(degmax);
	for (int j = 0; j < p->part_size[i] - 1; ++j)
		ans += compare_lists_bound(p->degree[p->parts[i][j]],
					   p->degree[p->parts[i][j + 1]]);
	return 3 + ans;
}

void find_stable_partition(const struct graph *g, struct partition *p, ll *bound) {
	int n = g->nb_nodes;
	*bound += malloc_wrapper_bound();
	int *succ_node_part = malloc_wrapper(n * sizeof *succ_node_part);
	int has_changed = 0;
	*bound += n * sizeof *p->node_part;
	memcpy(succ_node_part, p->node_part, n * sizeof *p->node_part);
 
	do {
		int prev_nb_parts = p->nb_parts;
		has_changed = 0;
		*bound += 2 + sort_adjacency_lists_bound(g->nb_nodes, p);
		sort_adjacency_lists(n, p);
	
		for (int i = 0; i < prev_nb_parts; ++i) {
			int degmax = 0;
			for (int j = 0; j < p->part_size[i]; ++j)
				degmax = max(degmax, p->degree[p->parts[i][j]]);
			*bound += stabilize_part_bound(p, i, degmax);
			has_changed |= stabilize_part(p, succ_node_part, i);
		}

		*bound += g->nb_nodes * sizeof *p->node_part;
		memcpy(p->node_part, succ_node_part, n * sizeof *p->node_part);
	} while (has_changed);

	*bound += 1;
	free(succ_node_part);
} 

#if 0
int find_stable_partition_bound(const struct graph *g, const struct partition *p) {
	int ans = malloc_wrapper_bound();
	ans += g->nb_nodes * sizeof *p->node_part;
	int degmax = 0;
	for (int i = 0; i < g->nb_nodes; ++i)
		degmax = max(degmax, g->deg[i]);

	for (int i = 0; i < g->nb_nodes; ++i) {
		ans += sort_adjacency_lists_bound(g->nb_nodes, degmax);
		for (int i = 0; i < p->nb_parts; ++i)
			ans += stabilize_part_bound(p, i, degmax);
		ans += g->nb_nodes * sizeof *p->node_part;
	}

	return ans;
}
#endif

/* Return 1 if p1 and p2 are incompatible partitions, 0 otherwise */
int are_incompatible(const struct partition *p1, const struct partition *p2) {
	if (p1->nb_parts != p2->nb_parts)
		return 1;

	for (int i = 0; i < p1->nb_parts; ++i) {
		int u = p1->parts[i][0], v = p2->parts[i][0];

		if (compare_lists(p1, p1->adjl[u], p1->degree[u], p2,
			          p2->adjl[v], p2->degree[v]) != 0)
			return 1;
	}

	return 0;
}

ll are_incompatible_bound(const struct partition *p1, const struct partition *p2) {
	if (p1->nb_parts != p2->nb_parts)
		return 1;
	
	int ans = 0;
	for (int i = 0; i < p1->nb_parts; ++i)
		ans += compare_lists_bound(p1->degree[p1->parts[i][0]],
				           p2->degree[p2->parts[i][0]]);
	return 0; 
}

/*
 * Create a new part with the only element found at the i-th position
 * in the ipart-th part
 */
void isolate(struct partition *p, int ipart, int i) {
	if (p->part_size[ipart] == 1)
		return;

	p->parts[p->nb_parts][0] = p->parts[ipart][i];
	p->node_part[p->parts[ipart][i]] = p->nb_parts;
	p->part_size[p->nb_parts] = 1;

	for (int j = i; j < p->part_size[ipart] - 1; ++j)
		p->parts[ipart][j] = p->parts[ipart][j + 1];

	p->part_size[ipart]--;
	p->nb_parts++;
}

ll isolate_bound(const struct partition *p, int ipart, int i) {
	if (p->part_size[ipart] == 1)
		return 1;
	return 5 + p->part_size[ipart] - i; 
}

/*
 * Find the part with the smallest cardinal in p, which doesn't contain an already
 * matched node
 */
int smallest_part(const struct partition *p, const int *e) {
	int imin = -1;

	for (int i = 0; i < p->nb_parts; ++i) {
		if (p->part_size[i] == 1) {
			if (!e[p->parts[i][0]])
				imin = i;
		} else if (imin == -1 || (p->part_size[i] < p->part_size[imin])) {
			imin = i;
		}
	}

	return imin;
}

ll smallest_part_bound(const struct partition *p) {
	return 2 + 3 * p->nb_parts; 
}

void copy_partition(int n, struct partition *p, const struct partition *old) {
	p->nb_parts = old->nb_parts;
	for (int u = 0; u < n; ++u) {
		p->degree[u] = old->degree[u];
		p->part_size[u] = old->part_size[u];
		p->node_part[u] = old->node_part[u];

		for (int v = 0; v < n; ++v) {
			p->parts[u][v] = old->parts[u][v];
//			p->adjl[u][v] = old->adjl[u][v];
		}
	}
}

int copy_partition_bound(int n) {
	return 1 + 3 * n + n * n;
}

/* Release a partition associated with g */
void free_partition(int n, struct partition *p) {
	for (int u = 0; u < n; ++u)
		free(p->adjl[u]);

	free(p->adjl);

	for (int u = 0; u < n; ++u)
		free(p->parts[u]);

	free(p->parts);
	free(p->node_part);
	free(p->part_size);
	free(p->degree);
}

ll free_partition_bound(int n) {
	return 6 + 2 * n;
}

#if 0
/* Debug function: print a partition */
void print_partition(struct partition p) {
	puts("-------------------------------------------------------");
	for (int i = 0; i < p.nb_parts; ++i) {
		printf("Part %d %d ", i, p.part_size[i]);
		for (int j = 0; j < p.part_size[i]; ++j) printf("%d ", p.parts[i][j]);
		puts("");
	}
	puts("-------------------------------------------------------");
}

#endif



int test_all_alone(struct partition *p1, struct partition *p2, const struct graph *g1,
		   const struct graph *g2, int *a, int *e, int k, ll *bound) {
	int n = g1->nb_nodes;
	struct partition n1 = create_partition(g1); copy_partition(n, &n1, p1);
	struct partition n2 = create_partition(g2); copy_partition(n, &n2, p2);
	*bound += 2 * copy_partition_bound(n) + 2 * copy_partition_bound(n);

	for (int i = 0; i < p1->nb_parts; ++i)
		if (p1->part_size[i] == 1) {
			int u = p1->parts[i][0];

			if (e[u])
				continue;

			a[u] = p2->parts[i][0];
			e[u] = 1;
			*bound += 5 + isolate_bound(&n1, i, 0) + isolate_bound(&n2, i, 0);
			isolate(&n1, i, 0);
			isolate(&n2, i, 0);
			k++;
		}
	
	*bound += is_isomorphism_partial_bound(g1->nb_nodes);
	int ans = is_isomorphism_partial(g1, g2, a, e) &&
		weisfeiler_lehman_from(&n1, &n2, g1, g2, a, e, k, bound);
	*bound += 2 * free_partition_bound(n);
	free_partition(n, &n1); free_partition(n, &n2);
	return ans;
}

int test_one(int imin, struct partition *p1, struct partition *p2, const struct graph *g1,
	     const struct graph *g2, int *a, int *e, int k, ll *bound) {
	int u = p1->parts[imin][0];
	int n = g1->nb_nodes;
	struct partition n1 = create_partition(g1);
	struct partition n2 = create_partition(g2);
	*bound += 2 * create_partition_bound(n);

	for (int i = 0; i < p1->part_size[imin]; ++i) {
		int v = p2->parts[imin][i];
		a[u] = v;
		*bound += 2;

		if (is_isomorphism_partial(g1, g2, a, e)) {
			copy_partition(n, &n1, p1); copy_partition(n, &n2, p2);
			*bound += 2 * copy_partition_bound(n);
			isolate(&n1, imin, 0); isolate(&n2, imin, i);
			*bound += isolate_bound(&n1, imin, 0) + isolate_bound(&n2, imin, i);

			if (weisfeiler_lehman_from(&n1, &n2, g1, g2, a, e, k + 1, bound)) {
				*bound += 2 * free_partition_bound(n);
				free_partition(n, &n1); free_partition(n, &n2);
				return 1;
			}
		}
	}

	*bound += 2 * free_partition_bound(n);
	free_partition(n, &n1); free_partition(n, &n2);
	return 0;
}

/* Recursive Weisfeiler-Lehman algorithm */
/* p1 (resp. p2) is the partition constructed so far in g1 (resp. g2) */
/* a is the current (partial) isomorphism */
/* e is the set of all elements who have already been set in a */
/* k is the total number of set elements */
int weisfeiler_lehman_from(struct partition *p1, struct partition *p2, const struct graph *g1,
 			   const struct graph *g2, int *a, int *e, int k, ll *bound) {
	*bound += 1;
	if (k == g1->nb_nodes)
		return 1;
	//assert(k < g1->nb_nodes);

	find_stable_partition(g1, p1, bound);
	find_stable_partition(g2, p2, bound);

	*bound += are_incompatible_bound(p1, p2);
	if (are_incompatible(p1, p2))
		return 0;
		
	*bound += smallest_part_bound(p1);
	int imin = smallest_part(p1, e);

	if (p1->part_size[imin] == 1)
		return test_all_alone(p1, p2, g1, g2, a, e, k, bound); 

	int u = p1->parts[imin][0];
	e[u] = 1;
	*bound += 2;

	if (test_one(imin, p1, p2, g1, g2, a, e, k, bound))
		return 1;

	*bound += 2;
	e[u] = 0;
	return 0;
}

/* Create a new partition associated with g */
struct partition create_partition(const struct graph *g) {
	int n = g->nb_nodes;
	struct partition p;
	p.part_size = calloc_wrapper(n, sizeof *p.part_size);
	p.degree = malloc_wrapper(n * sizeof *p.degree);
	p.node_part = calloc_wrapper(n, sizeof *p.node_part);
	p.parts = malloc_wrapper(n * sizeof *p.parts);
	
	for (int u = 0; u < n; ++u)
		p.parts[u] = malloc_wrapper(n * sizeof *p.parts[u]);

	p.adjl = malloc_wrapper(n * sizeof *p.adjl);

	for (int u = 0; u < n; ++u)
		p.adjl[u] = malloc_wrapper(n * sizeof *p.adjl[u]);

	p.nb_parts = 1;
	p.part_size[0] = n;

	for (int u = 0; u < n; ++u)
		p.parts[0][u] = u;

	for (int u = 0; u < n; ++u) {
		p.degree[u] = 0;
		for (int v = 0; v < n; ++v)
			if (g->adj[u][v])
				p.adjl[u][p.degree[u]++] = v;
	}

	return p;
}

ll create_partition_bound(int n) {
	int ans = 2 * n + 2 + 2 * n * n;
	ans += 2 * (n + 3) * malloc_wrapper_bound() + 2 * calloc_wrapper_bound();
	return ans;
}

/* General Weisfeiler-Lehman algorithm */
int weisfeiler_lehman(const struct graph *g1, const struct graph *g2, int *a, ll *bound) {
	int n = g1->nb_nodes;
	*bound += create_partition_bound(g1->nb_nodes) + create_partition_bound(g2->nb_nodes);
	struct partition p1 = create_partition(g1), p2 = create_partition(g2);
	find_stable_partition(g1, &p1, bound); find_stable_partition(g2, &p2, bound);
	*bound += calloc_wrapper_bound();
	int *e = calloc_wrapper(n, sizeof *e);
#if 0
	print_partition(p1);
	print_partition(p2);
#endif
//	for (int i = 0; i < p1.nb_parts; ++i)
//		fprintf(stderr, "%d ", p1.part_size[i]);
//	fprintf(stderr, "\n");
	int ans = weisfeiler_lehman_from(&p1, &p2, g1, g2, a, e, 0, bound);
	*bound += 3;
	free(e);
	free_partition(n, &p1);
	free_partition(n, &p2);
 	return ans;
}

/* Return 1 if g1 and g2 are isomorphic, 0 otherwise */
/* In the first case, a will contain the isomorphism at the end of simple_backtrack(g1, g2, a, 0) */
/* Algorithm: simple backtrack */
int backtrack_simple(const struct graph *g1, const struct graph *g2, int *a, int u, ll *bound) {
	int n = g1->nb_nodes;

	*bound += 1;
	if (u == n)
		return 1;

	for (int v = 0; v < n; ++v) {
		int possible = 1;

		for (int i = 0; i < u; ++i) {
			*bound += 2;
			if (a[i] == v || g1->adj[u][i] != g2->adj[v][a[i]] ||
			    g1->adj[i][u] != g2->adj[a[i]][v]) {
				possible = 0;
				break;	
			}
		}

		*bound += 2;
		if (possible) {
			a[u] = v;
			if (backtrack_simple(g1, g2, a, u + 1, bound))
				return 1;
		}
	}

	return 0;
}

/* Same as above */
/* Algorithm: backtrack with degree partition */
int backtrack_degree(const struct graph *g1, const struct graph *g2, int *a, int u, ll *bound) {
	*bound += 1;
	int n = g1->nb_nodes;

	if (u == n)
		return 1;

	for (int v = 0; v < n; ++v) {
		*bound += 1;
		if (g1->deg[u] != g2->deg[v])
			continue;

		int possible = 1;

		for (int i = 0; i < u; ++i) {
			*bound += 2;
			if (a[i] == v || g1->adj[u][i] != g2->adj[v][a[i]] ||
			    g1->adj[i][u] != g2->adj[a[i]][v]) {
				possible = 0;
				break;
			}
		}

		*bound += 2;
		if (possible) {
			a[u] = v;
			if (backtrack_degree(g1, g2, a, u + 1, bound))
				return 1;
		}
	}
	
	return 0;
} 

/* Read a graph in the specified input format */
struct graph *read_graph(void) {
	struct graph *g = malloc_wrapper(sizeof *g);

	if (scanf("%d%d", &g->nb_nodes, &g->nb_edges) != 2) {
		fprintf(stderr, "Wrong input\n");
		exit(EXIT_FAILURE);
	}

	g->adj = malloc_wrapper(g->nb_nodes * sizeof *g->adj);
	g->deg = malloc_wrapper(g->nb_nodes * sizeof *g->deg);

	for (int u = 0; u < g->nb_nodes; ++u)
		g->adj[u] = calloc_wrapper(g->nb_nodes, sizeof *g->adj[u]);

	for (int u = 0; u < g->nb_nodes; ++u) {
		if (scanf("%d", &g->deg[u]) != 1) {
			fprintf(stderr, "Wrong input\n");
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i < g->deg[u]; ++i) {
			int v;

			if (scanf("%d", &v) != 1) {
				fprintf(stderr, "Wrong input\n");
				exit(EXIT_FAILURE);
			}

			g->adj[u][v] = 1;
		}
	}

	return g;
}

/* Release the memory dynamically allocated for the storage of the graph */
void free_graph(struct graph *g) {
	for (int i = 0; i < g->nb_nodes; ++i)
		free(g->adj[i]);

	free(g->deg);
	free(g->adj);
	free(g);
}

/* Print usage and exit */
void usage(const char *name) {
	fprintf(stderr, "Usage: %s [b|d|w] graph.in [-bound]", name);
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
	enum { WEISFEILER, SIMPLE, DEGREE } type = WEISFEILER;
	int print_bound = 0;

	if (argc < 2)
		usage(argv[0]);
	
	int nextarg = 2;
	if (strcmp(argv[1], "b") == 0) type = SIMPLE;
	else if (strcmp(argv[1], "d") == 0) type = DEGREE;
	else if (strcmp(argv[1], "w") == 0) type = WEISFEILER;
	else nextarg = 1;

	if (freopen(argv[nextarg], "r", stdin) == NULL) {
		perror(argv[nextarg]);
		exit(EXIT_FAILURE);
	}

	++nextarg;

	if (argc > nextarg && strcmp(argv[nextarg], "-bound") == 0)
		print_bound = 1;

	struct graph *g1 = read_graph(); 
	struct graph *g2 = read_graph();

	if (g1->nb_nodes != g2->nb_nodes) {
		if (print_bound) puts("0");
		puts("non");
		return 0;
	}

	int *a = calloc_wrapper(g1->nb_nodes, sizeof *a);

	ll bound = 0;
	
	int result = 	((type == WEISFEILER && weisfeiler_lehman(g1, g2, a, &bound)) ||
			 (type == SIMPLE && backtrack_simple(g1, g2, a, 0, &bound)) ||
			 (type == DEGREE && backtrack_degree(g1, g2, a, 0, &bound)));
	if (print_bound)
		printf("%lld\n", bound);

	if (!result) {
		puts("non");
	} else {
		puts("oui");
		assert(is_isomorphism(g1, g2, a));
		for (int i = 0; i < g1->nb_nodes; ++i)
			printf("%d ", a[i]);
		puts("");
	}

	free(a);
	free_graph(g1);
	free_graph(g2);
	return EXIT_SUCCESS;
}
