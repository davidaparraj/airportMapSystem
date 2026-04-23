#ifndef UNIONFIND_HPP
#define UNIONFIND_HPP

#include <vector>

class UnionFind {
public:
    UnionFind(int n);
    int find(int x);
    bool unite(int x, int y); // returns false if set is already same

private:
    std::vector<int> parent;
    std::vector<int> rank;
};



#endif