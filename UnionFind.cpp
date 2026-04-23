#include "UnionFind.hpp"



UnionFind::UnionFind(int n) {
    parent.assign(n, 0);
    rank.assign(n, 0);
    for (int i = 0; i < n; i++) {
        parent[i] = i; // each node is its own root initially
    }
}

int UnionFind::find(int x) {
    // path compression - flattens the tree
    if (parent[x] != x) {
        parent[x] = find(parent[x]);
    }
    return parent[x];
}

bool UnionFind::unite(int x, int y) {
    int rootX = find(x);
    int rootY = find(y);

    if (rootX == rootY) return false; // already in same set, adding edge would create cycle

    // union by rank - attach smaller tree under larger tree
    if (rank[rootX] < rank[rootY]) {
        parent[rootX] = rootY;
    } else if (rank[rootX] > rank[rootY]) {
        parent[rootY] = rootX;
    } else {
        parent[rootY] = rootX;
        rank[rootX]++;
    }

    return true;
}