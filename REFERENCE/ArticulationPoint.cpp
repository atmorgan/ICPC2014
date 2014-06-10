#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;
/*
  Articulation Point: O(|V|+|E|).
  by Baasanbat Purevjal.
  Uses adjacency list to represent graph
  Do not forget to 
  ad[i].push_back(j) and ad[j].push_back(i) bidercation iff.
  art[i] true if vertex i is an articulation point.
  V - number of vertexes. 0, 1, ..., V-1.
  E - number of edges.
  Input format:
  4 3
  0 1
  1 2
  2 3
  0 0
  Output:
  1
  2
*/
// BEGIN
#define N 400001
#define pb push_back
int num[N] = {0}, low[N] = {0};
int visit[N] = {0};
int parentt[N] = {0}; 
int V, E;        
vector<int> ad[N];
int art[N] = {0};
int counter;
int root, child = 0;

void findartd(int ver) {
    visit[ver] = 1;
	  low[ver] = num[ver] = counter++;
    vector<int>::iterator it;
    for (it = ad[ver].begin(); it < ad[ver].end(); it++)
         if (visit[*it] == 0) {
             if (root == ver)
                 child++;
             parentt[*it] = ver;
             int tm = *it;
             findartd(tm);
	           if (low[*it] >= num[ver]) {

                 if (art[ver] == 0 && root != ver) 
                     art[ver] = 1;

             }
             low[ver]=min(low[ver],low[*it]);	
             
          } else 
               if (parentt[ver] != *it)
                   low[ver] = min(low[ver], num[*it]);	
} 
int main() {
	while (true) {
		scanf("%d %d", &V, &E);
		if(V == 0 && E == 0) break;
    for (int i = 0; i < E; i++) {
        int s, t;	
        scanf("%d %d", &s, &t);
        ad[s].pb(t);
        ad[t].pb(s);
    }
    for (int i = 0; i < V; i++) 
        if (!visit[i]) {
            counter = 1;
            child = 0;
            root = i;
            findartd(root);
            if (child > 1) art[root] = 1;
        }
    for (int i = 0; i < V; i++)
         if(art[i] == 1)
            printf("%d\n", i); 
    for (int i = 0; i < V; i++) ad[i].clear();
    memset(num,0,sizeof(num));
	  memset(visit,0,sizeof(visit));
	  memset(parentt,0,sizeof(parentt));
    memset(art,0,sizeof(art));
	  memset(low,0,sizeof(low));

  }//while(true)..
 
return 0; }
// END
