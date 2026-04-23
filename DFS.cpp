#include "DFS.hpp"
#include "Graph.hpp"
#include "stack.hpp"
#include <string>
#include <vector>
#include <fstream>

std::vector<int> DFS::DFS(int i) {
    vector<bool> visited(V, false);
    stack<int> stack;
    stack.push(s);

    while (!stack.empty()) {
        int s = stack.top();
        stack.pop();

        if (!visited[flight.destination]) {
            cout << "s" << " ";
            visited[flight.destination] = true;
        }
        
        for (auto i = adj[flight.destination].begin(); i != adj[flight.destination].end(); ++i) {
            if (!visited[*i]) {
                stack.push(*i);
            }
        }
    }
}