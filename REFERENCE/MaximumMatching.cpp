#include <vector>

using namespace std;

// BEGIN
// This code performs maximum bipartite matching.
// It has a heuristic that will give excellent performance on complete graphs
// where rows <= columns.
//
//   INPUT: w[i][j] = cost from row node i and column node j or NO_EDGE
//   OUTPUT: mr[i] = assignment for row node i or -1 if unassigned
//           mc[j] = assignment for column node j or -1 if unassigned
//
//   BipartiteMatching returns the number of matches made.
//
// Contributed by Andy Lutomirski.

typedef vector<int> VI;
typedef vector<VI> VVI;

const int NO_EDGE = -(1<<30);  // Or any other value.

bool FindMatch(int i, const VVI &w, VI &mr, VI &mc, VI &seen)
{
  if (seen[i])
    return false;
  seen[i] = true;
  for (int j = 0; j < w[i].size(); j++) {
    if (w[i][j] != NO_EDGE && mc[j] < 0) {
      mr[i] = j;
      mc[j] = i;
      return true;
    }
  }
  for (int j = 0; j < w[i].size(); j++) {
    if (w[i][j] != NO_EDGE && mr[i] != j) {
      if (mc[j] < 0 || FindMatch(mc[j], w, mr, mc, seen)) {
	mr[i] = j;
	mc[j] = i;
	return true;
      }
    }
  }
  return false;
}

int BipartiteMatching(const VVI &w, VI &mr, VI &mc)
{
  mr = VI (w.size(), -1);
  mc = VI(w[0].size(), -1);
  VI seen(w.size());

  int ct = 0;
  for(int i = 0; i < w.size(); i++)
    {
      fill(seen.begin(), seen.end(), 0);
      if (FindMatch(i, w, mr, mc, seen)) ct++;
    }
  return ct;
}
//END
