#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <iostream>

using namespace std;

// BEGIN
// An implementation of Aho-Corasick dictionary matching algorithm
// Taken directly from the paper
// See also: https://en.wikipedia.org/wiki/Aho-Corasick_algorithm

typedef vector<size_t> VI;
typedef vector<string> VS;

struct node {
    unordered_map<char, node*> g;
    node* f;    // pointer to node with largest strict suffix of current node in automaton
    node* output = NULL;    // pointer to dictionary suffix, the next node with isWord=true by
                            // following f pointers
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

// creates Aho-Corasick automaton from a dictionary
node* ConstructAutomaton(VS &dictionary) {
    size_t n = 0;
    node *root = new node(n++);
    for (size_t i = 0; i < dictionary.size(); ++i) {
        enter(dictionary[i], root, n);
    }
    construct_f(root);
    return root;
}

// returns a pointer to the next node after the character c is encountered
node* advance(node *state, char c) {
    while (state->g.count(c) == 0 && state->num != 0) state = state->f;
    if (state->g.count(c) != 0)
        state = state->g[c];
    return state;
}

// prints all matches of dictionary words in the given query string
void PrintMatches(node *root, string x) {
    node *state = root;
    for (size_t i = 0; i < x.size(); ++i) {
        state = advance(state, x[i]);
        node *out = state;
        while (true) {
            if (out->isWord) cout << i << " " << out->num << " " << endl;
            if (out->output == NULL) break;
            out = out->output;
        }
    }
}

// END

string randString(size_t length) {
    string S(length, '?');
    for (size_t i = 0; i < length; ++i) {
        S[i] = 'a' + rand()%26;
    }
    return S;
}

int main() {
    srand(0);
    {
        VS dictionary = {"a", "ab", "bab", "bc", "bca", "c", "caa"};
        node *root = ConstructAutomaton(dictionary);
        
        cout << "Expected:\n0 1\n1 2\n2 6\n2 8\n3 8\n4 1\n5 2" << endl;
        cout << "Actual:" << endl;

        PrintMatches(root, "abccab");
    }
    {
        VS dictionary;
        for (size_t i = 0; i < 10000; ++i) {
            dictionary.push_back(randString(1000));
        }

        cout << "Speed Test" << endl;
        cout << "Constructing Automaton..." << endl;
        node *root = ConstructAutomaton(dictionary);
        
        cout << "Complete. Querying..." << endl;
        for (size_t i = 0; i < 10; ++i) {
            PrintMatches(root, randString(1000000));
        }
        cout << "Complete" << endl;
    }
    // code has been successfully tested on SWERC '16 "Passwords"
}
