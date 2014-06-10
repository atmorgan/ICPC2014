/*
	Minimum Spanning Tree: Krsukal.
	Complexity: O(NlogN).
	by Baasanbat Purevjal.
	mst contains all edges of MST.
	ret is sum of all weight of our MST.
*/
// BEGIN
#define mp make_pair
#define pb push_back
#define Max 1000100
using namespace std;
vector< pair<int, pair<int, int> > >v, mst;
int parent[Max], rank[Max];

void makeSet(int n) {
	for (int i = 0; i < n; i++)
		parent[i] = i,
		rank[i] = 0;
}

int find(int x) {
	if (x != parent[x])
		parent[x] = find(parent[x]);
	return parent[x];
}

void merge(int x, int y) {
	int Px = find(x);
	int Py = find(y);

	if (rank[Px] > rank[Py])
		parent[Py] = Px;
	else
		parent[Px] = Py;

	if (rank[Px] == rank[Py])
		++rank[Py];

}

int main(int argc, char *argv[]) {
    //freopen("in.txt", "r", stdin);
    //freopen("out.txt", "w", stdout);

    int n, m;
    scanf("%d%d", &n, &m);
    makeSet(n);
    for (int i = 0; i < m; i++) {
    	int x, y, w;
    	scanf("%d%d%d", &x, &y, &w);
    	x--, --y;
    	v.pb(mp(w, mp(x, y)));
    }
    sort(v.begin(), v.end());

    long long ret = 0ll;

    for (int i = 0, k = 0; k < n-1; i++) {
    	int x = find(v[i].second.first);
    	int y = find(v[i].second.second);
    	if (x != y) {
    		mst.pb(v[i]);
    		ret += 1ll*v[i].first;
    		merge(x, y);
    		k++;
    	}
    }

    printf("%lld\n", ret);

    return 0;
}
// END
