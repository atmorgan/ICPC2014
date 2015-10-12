#include <vector>

using namespace std;

// BEGIN
// This code performs maximum (cardinality) bipartite matching.
// Does not support weighted edges.
//
// Running time: O(|E| |V|) -- often much faster in practice
//
//   INPUT: adj_list[i][j] = edge between row node i and column node adj_list[i][j]
//          mr[i] = vector of size #rows, initialized to -1
//          mc[j] = vector of size #columns, initialized to -1
//          
//   OUTPUT: mr[i] = assignment for row node i, -1 if unassigned
//           mc[j] = assignment for column node j, -1 if unassigned
//           function returns number of matches made

typedef vector<int> VI;
typedef vector<VI> VVI;
typedef vector<bool> VB;

bool FindMatch(int i, const VVI &adj_list, VI &mr, VI &mc, VB &seen) {
  for (int j = 0; j < adj_list[i].size(); j++) {
    int item = adj_list[i][j];
    if (!seen[item]) {
      seen[item] = true;
      if (mc[item] < 0 || FindMatch(mc[item], adj_list, mr, mc, seen)) {
        mr[i] = item;
        mc[item] = i;
        return true;
      }
    }
  }
  return false;
}

// mr should be a vector of size number of row items, initialized to -1
// mc should be a vector of size number of column items, initialized to -1
int BipartiteMatching(const VVI &adj_list, VI &mr, VI &mc) {
  int ct = 0;
  for (int i = 0; i < adj_list.size(); i++) {
    VB seen(mc.size(), false);
    if (FindMatch(i, adj_list, mr, mc, seen)) ct++;
  }
  return ct;
}
// END

int main() {
    //TODO: implement tests
    return 0;
}
