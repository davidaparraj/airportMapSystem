#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>

#include "Graph.hpp"
#include "Heap.hpp"
#include "UnionFind.hpp"


int main() {
    // Create a graph using adjacency list

    /* Open file and parse the following values:
        As a node (airport characteristics)
            - Airport code
            - Airport city
        As an edge (flight characteristics)
        - Distance
        - Cost

        Add info into the graph
    */
    std::ifstream infile("airports.csv");
    if(!infile) {
        std::cerr << "Error. Could not open file.\n";
        return 1;
    }
    // Ignores first line (only contains headers)
    infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


    // Task 1) Create graph
    // Creates graph to store data from csv file
    Graph graph;
    graph.parseAndBuild(infile);

    // Task 2) Find shortest path by distance
    // graph.printShortestPath("IAD", "MIA");
    graph.printShortestPath("ATL", "BOS");
    graph.printShortestPath("DFW", "MCO");

    // Taks 3) Shortest paths to all airports in a state
    graph.printShortestPathBySate("DFW", "FL");

    // Task 4) Shortest path with a given amount of stops
    graph.printPathWithStops("IAD", "MIA", 3);
    graph.printPathWithStops("PIT", "ACT", 4);

    // Task 5)
    graph.printConnectionCounts();

    // Task 6) Create an undirect graph_u from original graph
    Graph G_u = graph.buildUndirected();

    // Task 7) Prims Minimal Spanning Tree Algorithm
    G_u.primMST();

    // Task 8) Kruskal's Minimal Spanning Tree Algorithm
    G_u.kruskalMST();


    return 0;
}