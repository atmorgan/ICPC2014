// From http://web.mit.edu/~ecprice/acm/acm07/
// Bridge finding!

#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int st, en, id;
} edg;

edg edges[200000];
int isbridge[100000];
int bridges[100000];
int n,m,startloc[10001], dfn[10000], l[10000], cnt;

int f_cmp (const void *a, const void *b) {
  edg c,d;

  c = *(edg *)a;
  d = *(edg *)b;
  if (c.st < d.st) return -1;
  if (c.st > d.st) return 1;
  return 0 ;
}

void dfs (int node, int parID) {
  int i;
  dfn[node] = ++cnt;
  l[node] = dfn[node];
  for (i = startloc[node]; i < startloc[node+1]; i++) { 
    if (edges[i].id == parID)
      continue;
    if (!dfn[edges[i].en]) {
      dfs(edges[i].en, edges[i].id);
      if (l[edges[i].en] <= dfn[node]) {
        isbridge[edges[i].id] = 0;
        if (l[edges[i].en] < l[node])
          l[node] = l[edges[i].en];
      }
    } else {
      isbridge[edges[i].id] = 0;
      if (dfn[edges[i].en] < l[node])
        l[node] = dfn[edges[i].en];
    }
  }
}

int main() {
  int t,ll,i,ct;

  scanf ("%d", &t);
  for (ll = 0; ll < t; ll++) {
    scanf (" %d %d", &n, &m);

    cnt = 0;
    for (i = 0; i < n; i++)
      dfn[i] = 0; 
    for (i = 0; i < m; i++)
      isbridge[i] = 1;
    for (i = 0; i < 2 * m; i+=2) {
      scanf (" %d %d", &edges[i].st, &edges[i].en);
      edges[i].st--; edges[i].en--;
      edges[i].id = i / 2;
      edges[i+1].st = edges[i].en;
      edges[i+1].en = edges[i].st;
      edges[i+1].id = edges[i].id;
    }
    qsort((void *)edges, 2 * m, sizeof(edg), f_cmp);  
    startloc[0] = 0;
    for (i = 1; i < 2 * m; i++) {
      if (edges[i].st != edges[i-1].st)
        startloc[edges[i].st] = i;
    }
    startloc[n] = 2 * m;
    dfs (0, -1);
    ct = 0;
    for (i = 0; i < m; i++)
      if (isbridge[i]) {
        bridges[ct] = i;
        ct++;
      }
    printf ("%d\n",ct);
    if (ct != 0) {
      printf ("%d",bridges[0]+1);
      for (i = 1; i < ct; i++)
        printf(" %d",bridges[i]+1);
      printf ("\n");
    }
    if (ll != t - 1)
      printf ("\n"); 
  }
  return 0;
}
