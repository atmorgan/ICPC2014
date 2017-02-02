#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <iostream>

using namespace std;

// BEGIN
// An implementation of Aho-Corasick dictionary matching algorithm
// Taken directly from the paper

typedef vector<size_t> VI;
typedef vector<string> VS;

struct node {
    unordered_map<char, node*> g;
    node* f;
    node* output = NULL;
    bool isWord = false;
    size_t num;

    node(size_t num) : num(num) {}
    node() {}
};

void enter(string S, node *root, size_t &n) {
    node *state = root;
    size_t j = 0;
    while (state->g.count(S[j]) != 0) {
        state = state->g[S[j]];
        ++j;
    }
    for (; j < S.size(); ++j) {
        state->g[S[j]] = new node(n++);
        state = state->g[S[j]];
    }
    state->isWord = true;
}

void construct_f(node *root) {
    queue<node*> q;
    for (auto it = root->g.begin(); it != root->g.end(); ++it) {
        q.push(it->second);
        it->second->f = root;
    }
    while (!q.empty()) {
        node *r = q.front(); q.pop();
        for (auto it = r->g.begin(); it != r->g.end(); ++it) {
            q.push(it->second);
            node *state = r->f;
            while (state->g.count(it->first) == 0 && state->num != 0)
                state = state->f;
            if (state->g.count(it->first) != 0)
                it->second->f = state->g[it->first];
            else
                it->second->f = root;
            if (it->second->f->isWord)
                it->second->output = it->second->f;
            else   // may assign NULL pointer
                it->second->output = it->second->f->output;
        }
    }
}

node* ConstructAutomaton(VS &dictionary) {
    size_t n = 0;
    node *root = new node(n++);
    for (size_t i = 0; i < dictionary.size(); ++i) {
        enter(dictionary[i], root, n);
    }
    construct_f(root);
    return root;
}

void PrintMatches(node *root, string x) {
    node *state = root;
    for (size_t i = 0; i < x.size(); ++i) {
        while (state->g.count(x[i]) == 0 && state->num != 0) state = state->f;
        if (state->g.count(x[i]) != 0)
            state = state->g[x[i]];
        node *out = state;
        while (true) {
            if (out->isWord) cout << i << " " << out->num << " " << endl;
            if (out->output == NULL) break;
            out = out->output;
        }
    }
}

// END

int main() {
    VS dictionary = {"a", "ab", "bab", "bc", "bca", "c", "caa"};
    node *root = ConstructAutomaton(dictionary);
    
    cout << "Expected:\n0 1\n1 2\n2 6\n2 8\n3 8\n4 1\n5 2" << endl;
    cout << "Actual:" << endl;

    PrintMatches(root, "abccab");

}
