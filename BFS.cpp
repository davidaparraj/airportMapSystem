#include "BFS.hpp"
#include "queue.hpp"
#include <string>
#include <vector>
#include <fstream>


vector  BFS::BFS(int s) {
    bool *visited = new bool[V];
    for(int i = 0; i < V; i++) {
        visited[i] = false;
    }
    list<int> queue;
    visited[s] = true;
    queue.push_back(s);
    list<int>::iterator i;

    while(!queue.empty()) {
        s = queue.front();
        cout << s << " ";
        queue.pop_front();
    }

    for(i = adj[s].begin(); i != adj[s].end(); i++) {
        if (!visited[*i]) {
            visited[*i] = true;
            queue.push_back(*i);
        }
    }
}