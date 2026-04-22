#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <vector>
#include <fstream>

// Represents one directed edge in the adjacency list
struct Flight {
    // Index of destination airport in airports[] (from graph class)
    int destination;
    // edge weight
    int distance;
    // edge weight
    int cost;
    
    Flight(int destination, int distance, int cost) : destination(destination), distance(distance), cost(cost) {}
};


// Airport: holds one airport node info and all outgoing edges
struct Airport {
    std::string code; // ex: ABE
    std::string city; // ex: "Detroit, MI"
    std::vector<Flight> adjacent; // Outgoing flights from this airport

    Airport(const std::string& code, const std::string& city) : code(code), city(city) {}
};


// Weighted directed graph implemented with adjacency list
class Graph {
public:
    // Reads csv file and builds graph
    void parseAndBuild(std::ifstream& infile);

    // Adds an airport to the graph
    void addNode(const std::string& code, const std::string& city);
    // Adds a directed flight from -> to  with both weights 
    void addEdge(const std::string& from, const std::string& to, int distance, int cost);

    /* --------- Algorithms -------------*/
    // Looks up index of an airport by its code. Return its index. Return -1 if not found
    int findNode(const std::string& code) const;

    /*Shortest Possible Path algorith (Dijksra) 
       Finds all of the Paths and adds them into the MinHeap
       Minheap sorts for shorest path staying at the root
       Dijkstra ends by pulling the root from the MinHeap*/
    std::vector<int> dijkstra(int source);
    // Returns total number of airports in the graph
    int size() const;
    // Dijkstra's algorithm which searched the nodes and determines the shortest path.
    std::vector<int> dijkstra(int source, std::vector<int>& prev, std::vector<int>& costDist);

    // Prints graph
    void print() const;
    // prints the shortest path given from the dijkstra algo
    void printShortestPath(const std::string& origin, const std::string& dest);




private: 
    // array containing all of the nodes (airports) in the graph
    std::vector<Airport> airports;
};

/* ---------- Helper functions ---------- */

// Splits values and stores them in tokens[]
bool splitCSVLine(const std::string& line, std::string tokens[]);


#endif