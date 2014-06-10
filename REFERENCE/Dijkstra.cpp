using namespace std;
// BEGIN
#define mp make_pair
#define pb push_back
#define Pair pair<int, int> // V W
#define xx first
#define yy second
#define Max 100010
struct cmp {
	bool operator() (const Pair &a, const Pair &b) const {
		return a.yy > b.yy;
	}
};
const long long INF = 12345678987654321LL;
priority_queue< Pair, vector< Pair >, cmp> q;
vector< Pair >v[Max];
int n, m;
bool used[Max];
int uu[Max];
long long d[Max];
void dfs(int i) {
	if (i == 0) {
		printf("1 ");
		return;
	}
	dfs(uu[i]);
	printf("%d ", i+1);
}
int main(int argc, char *argv[]) {
    scanf("%d%d", &n, &m);
    while (m--) {
    	int a, b, w;
    	scanf("%d%d%d", &a, &b, &w);
    	a--, --b;
    	v[a].pb(mp(b, w));
    	v[b].pb(mp(a, w));
    }
    uu[0] = 0; //from vertex 0.
    for (int i = 0; i < n; i++)
    	d[i] = INF;
    d[0] = 0ll;
    q.push(mp(0, 0));
    while (q.size()) {
    	int u = q.top().xx;
    	q.pop();
    	int sz = v[u].size();
    	for (int i = 0; i < sz; i++) {
    		int vv = v[u][i].xx;
    		int ww = v[u][i].yy;
    		
    		if(!used[vv] && d[u]+ww < d[vv]) {
    			d[vv] = d[u]+ww;
    			q.push(mp(vv, d[vv]));
    			uu[vv] = u;
    		}

    	}
    	used[u] = 1;
    }
    if (d[n-1] == INF)
    	printf("-1\n");
    else
    	dfs(n-1);
    return 0;
}
// END
