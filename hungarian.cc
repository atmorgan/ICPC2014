// From http://web.mit.edu/~ecprice/acm/acm07/
// Hungarian matching
// Min-cost matching
#include <stdio.h>
#include <string.h>

#define ABS(x) ((x)<0?-(x):(x))
typedef struct {
  int r,c;
} pos;

pos house[128], man[128]; 
int dist[128][128], newdist[128][128];
int m[128], h[128];
int x,y,nh,nm;
int mark, vm[128], vh[128];
char inp[100][100];

int calcdist (pos a, pos b) {
  return (ABS(a.r-b.r)+ABS(a.c-b.c));
}
int augment(int curman) {
  int i,j;

  vm[curman] = mark; 
  for (i = 0; i < nh; i++) {
    if (newdist[curman][i] == 0 && m[i] == -1) {
      m[i] = curman;
      h[curman] = i;
      return 1;
    }
  }
  for (i = 0; i < nh; i++) {
    if (newdist[curman][i] == 0 && vh[i] != mark) {
      vh[i] = mark;
      if (augment(m[i])) { h[curman] = i; m[i] = curman; return 1; }
    }
  }
  return 0;
}

int main() {
  int i,j,k,min,ans;

  scanf ("%d %d", &x, &y);
  while (x != 0 || y != 0) {
    nh = nm = 0;
    for (i = 0; i < x; i++) {
      scanf (" %s", inp[i]); 
      for (j = 0; j < y; j++) {
        if (inp[i][j] == 'H') {
          house[nh].r = i;
          house[nh].c = j;
          nh++;
        } else if (inp[i][j] == 'm') {
          man[nm].r = i;
          man[nm].c = j;
          nm++;
        }
      }
    }
    for (i = 0; i < nm; i++)
      for (j = 0; j < nh; j++)
        dist[i][j] = newdist[i][j] = calcdist(man[i],house[j]);
    mark = 0;
    for (i =0 ; i < nm; i++) {
      vm[i] = 0;
      h[i] = -1;
    }
    for (i =0 ; i < nh; i++) {
      vh[i] =0;
      m[i] = -1;
    }
    for (k = 0; k < nm; k++) {
      mark++;
      while (!augment(k)) {
        min = 1000000000;
        for (i = 0; i < nm; i++)
          if (vm[i] == mark) {
            for (j = 0; j < nh; j++)
              if (vh[j] != mark && newdist[i][j] != 0 && newdist[i][j] < min) {
                min = newdist[i][j];
              }
          }
        for (i = 0; i < nm; i++)
          if (vm[i] == mark) {
            for (j = 0; j < nh; j++)
              newdist[i][j] -= min;
          }
        for (j = 0; j < nh; j++)
          if (vh[j] == mark) {
            for (i = 0; i < nm; i++)
              newdist[i][j] += min;
          }
        mark++;
      }
    }
    ans =0 ;
    for (i = 0; i < nm; i++)
      ans += dist[i][h[i]]; 
    printf ("%d\n",ans);
    scanf ("%d %d", &x, &y);
        
  }
  return 0;
}
