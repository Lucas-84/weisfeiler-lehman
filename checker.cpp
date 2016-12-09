// Checker for stable partitioning
#include <algorithm>
#include <cstdio>
#include <map>
#include <set>
#include <vector>
#include <queue>
#include <cassert>
#include <cstdlib>
using namespace std;
#define mp make_pair
#define pb push_back
#define fst first
#define snd second
#define sz(a) int((a).size())
#ifndef M_PI
#	 define M_PI 3.14159265358979323846
#endif
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
const int INF = 1000 * 1000 * 1000;
const ll LLINF = 1ll << 53;
void relaxmax(int& r, int v) { r = max(r, v); }
void relaxmin(int& r, int v) { r = min(r, v); }

int main(int argc, char *argv[]) {
	FILE *fin = fopen(argv[1], "r");
	if (fin == NULL) {
		perror("fopen");
		return EXIT_FAILURE;
	}
	int N, M;
	fscanf(fin, "%d%d", &N, &M);
	vector<vi> adj(N);
	for (int u = 0; u < N; ++u) {
		int d;
		fscanf(fin, "%d", &d);
		for (int j = 0; j < d; ++j) {
			int v;
			fscanf(fin, "%d", &v);
			adj[u].pb(v);
		}
	}
	int id_part;
	vector<int> node_part(N);
	vector<vi> parts;
	while (scanf(" Part %d ", &id_part) == 1) {
		vi part;
		int partsz;
		scanf("%d", &partsz);
		for (int i = 0; i < partsz; ++i) {
			int u;
			scanf("%d", &u);
			node_part[u] = id_part;
			part.pb(u);
		}
		parts.pb(part);
	}
	vector<vi> hash(N);
	for (int u = 0; u < N; ++u) {
		for (int v: adj[u])
			hash[u].pb(node_part[v]);
		sort(hash[u].begin(), hash[u].end());
	}
	for (vi p: parts) {
		for (int i = 0; i < int(p.size()) - 1; ++i)
			if (hash[p[i]] != hash[p[i + 1]]) {
				printf("ERROR\n");
				printf("%d %d\n", p[i], p[i + 1]);
			}
	}
	fclose(fin);
	return 0;
}
