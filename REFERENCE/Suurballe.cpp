// BEGIN
// Suurballe's algorithm.
// finds smallest possible sum of weight of two disjoint paths from s to t.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <cctype>

#include <vector>
#include <map>
#include <queue>
#include <algorithm>

using namespace std;

typedef vector<int> VI;
typedef vector<VI> VVI;
typedef pair<int,int> PII;

//XXX XXX define me
//#define PRINT_PATH
//XXX XXX define me

#define MAX_E 99999 // should be 2*actual maximum
#define MAX_V 9999

int e_from[MAX_E], e_to[MAX_E], e_cap[MAX_E], e_flow[MAX_E], e_cost[MAX_E], e_dual[MAX_E];
VVI edges;
int s, t;

int back_e[MAX_V];
int cost[MAX_V];

int augment() {
    memset(cost, 0x10, sizeof(cost));
    priority_queue<PII> q;
    cost[s] = 0;
    q.push(make_pair(0,s));
    while( q.size() > 0 ) {
        PII cur_p = q.top(); q.pop();
        int cur = cur_p.second;
        int curcost = -cur_p.first;
        if( cost[cur] < curcost ) { continue; }
        if( cur == t ) {
            // augment path
            int v = t;
            #ifdef PRINT_PATH
            VI path; path.push_back(v/2);
            #endif
            while( v != s ) {
                int e = back_e[v];
                int d = e_dual[e];
                e_flow[e] += 1;
                e_flow[d] -= 1;
                v = e_from[e];
                #ifdef PRINT_PATH
                if( v/2 != *path.rbegin() ) { path.push_back(e_cost[e]); path.push_back(v/2); }
                #endif
            }
            #ifdef PRINT_PATH
            reverse(path.begin(), path.end()); printf("%d", path[0]);
            for( int i = 1; i < path.size(); i += 2 ) {
              printf(" --[%d]--> %d", path[i], path[i+1]);
            } puts("");
            #endif
            return curcost;
        }
        for( int i = 0; i < edges[cur].size(); ++i ) {
            int e = edges[cur][i];
            if( e_flow[e] >= e_cap[e] ) { continue; }
            int next = e_to[e];
            int nextcost = curcost + e_cost[e];
            if( cost[next] > nextcost ) {
                back_e[next] = e;
                cost[next] = nextcost;
                q.push(make_pair(-nextcost, next));
            }
        }
    }
    return -1;
}

void solve() {
    int v, e; if( scanf("%d%d", &v, &e) != 2 ) exit(0);
    edges = VVI(2*v,VI());
    for( int i = 0; i < v; ++i ) {
        int idx = 2*i;
        e_from[idx] = 2*i;
        e_to[idx] = 2*i+1;
        e_cost[idx] = e_cost[idx+1] = 0;
        e_from[idx+1] = e_to[idx];
        e_to[idx+1] = e_from[idx];
        e_cap[idx] = e_cap[idx+1] = 1;
        e_flow[idx] = 0; e_flow[idx+1] = 1;
        e_dual[idx] = idx+1; e_dual[idx+1] = idx;
        edges[e_from[idx]].push_back(idx);
        edges[e_from[idx+1]].push_back(idx+1);
    }
    for( int i = 0; i < e; ++i ) {
        int idx = 2*i+2*v;
        scanf("%d%d%d", &e_from[idx], &e_to[idx], &e_cost[idx]);
        e_from[idx] -= 1; e_to[idx] -= 1;
        e_from[idx] *= 2; e_to[idx] *= 2;
        e_from[idx] += 1;
        e_from[idx+1] = e_to[idx];
        e_to[idx+1] = e_from[idx];
        e_cost[idx+1] = -e_cost[idx];
        e_cap[idx] = e_cap[idx+1] = 1;
        e_flow[idx] = 0; e_flow[idx+1] = 1;
        e_dual[idx] = idx+1; e_dual[idx+1] = idx;
        edges[e_from[idx]].push_back(idx);
        edges[e_from[idx+1]].push_back(idx+1);
    }
    s = 1; t = 2*(v-1);
    int total = 0;
    for( int i = 0; i < 2; ++i ) {
        total += augment();
    }
    printf("%d\n" , total);
}
int main() {
    while( true ) solve();
}
// END
