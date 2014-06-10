/*
  Strongly Connected Components.
  Implementation - Kosaraju's Algorithm.
  Complexity - O(V+E).
*/
using namespace std;
// BEGIN
#define mp make_pair
#define pb push_back
#define MAXV 100
#define MAXE 100
struct edge {
  int e, 
  nxt;
};
int V, E;
edge e[MAXE], er[MAXE];
int sp[MAXV], spr[MAXV];
int group_cnt, group_num[MAXV];
bool v[MAXV];
int stk[MAXV];
void fill_forward(int x) {
  int i;
  v[x] = true;
  for(i = sp[x]; i; i = e[i].nxt) 
    if(!v[e[i].e]) 
      fill_forward(e[i].e);
  stk[++stk[0]] = x;
}
void fill_backward(int x) {
  int i;
  v[x] = false;
  group_num[x] = group_cnt;
  for (i = spr[x]; i; i = er[i].nxt) 
    if(v[er[i].e]) 
      fill_backward(er[i].e);
}
void add_edge(int v1, int v2) {//add edge v1->v2
  e[++E].e = v2; 
  e[E].nxt = sp[v1]; 
  sp[v1] = E;
  er[E].e = v1; 
  er[E].nxt = spr[v2]; 
  spr[v2] = E;
}
void SCC() {
  int i;
  stk[0] = 0;
  memset(v, false, sizeof(v));
  for(i = 1; i <= V;i++) 
    if(!v[i]) 
      fill_forward(i);
  group_cnt = 0;
  for(i = stk[0];i >= 1;i--) 
    if(v[stk[i]]) {
      group_cnt++; 
      fill_backward(stk[i]);
    }
}
int main() {
  int t;
  scanf("%d", &t);
  while (t--) {
    memset(stk, -1, sizeof(stk));
    memset(sp, -1, sizeof(sp));
    memset(spr, -1, sizeof(spr));
    int m;
    E = 0;
    scanf("%d%d", &V, &m);
    for (int i = 0; i < m; i++) {
      int x, y;
      scanf("%d%d", &x, &y);
      add_edge(x, y);
    }
    SCC();
    printf("%d\n", group_cnt);
    for (int i = 1; i <= V; i++)
      printf("%d stays in group %d\n", i, group_num[i]);
    printf("\n");
  }
}
// END
