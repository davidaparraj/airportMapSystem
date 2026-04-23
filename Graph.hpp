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
    // Looks up index of an airport by its code. Return its index. Return -1 if not found
    int findNode(const std::string& code) const;
    // Returns total number of airports in the graph
    int size() const;

    /* --------- Algorithms -------------*/

    // Dijkstra's algorithm which searched the nodes and determines the shortest path.
    void dijkstra(int source, std::vector<int>& prev, std::vector<int>& dist);


    // Task 2) prints the shortest path given from the dijkstra algorithm
    void printShortestPath(const std::string& origin, const std::string& dest);

    // Task 3) finds and prints shortest paths to all airports in one state. Also includes paths that have stops
    void printShortestPathBySate(const std::string& origin, const std::string& state);

    // Task 4) finds and prints the shortest path to a given airport with a predetermined amount of intermediate stops
    void findPathWithStops(int current, int dest, int stopsLeft, int currDist, int currCost, std::vector<int>& path, std::vector<bool>& visited, std::vector<int>& bestPath, int& bestDist, int& bestCost);
    void printPathWithStops(const std::string& origin, const std::string& dest, int stops);

    //BFS and DFS to be used in prims and kruskuls 
    std::vector<int> DFS(int source);
    std::vector<int> BFS(int source);

    
    // Task 5) Print the number of connections for every airport
    // recursive helper function for printPathWithStops
    void printConnectionCounts();

    // Task 6) Creates undirected Graph_u from original Graph G
    Graph buildUndirected() const;
    // Returns cost of edge from airport u to airport v, or -1 if it doesn't exist
    int getEdgeCost(int u, int v) const;

    // Task 7 and 8
    void primMST();
    void kruskalMST();



private: 
    // array containing all of the nodes (airports) in the graph
    std::vector<Airport> airports;


};

/* ---------- Helper functions ---------- */

// Splits values and stores them in tokens[]
bool splitCSVLine(const std::string& line, std::string tokens[]);
// Returns a city's state
std::string getState(const std::string& city);
int getTotalCost(const std::vector<int> path, const std::vector<Airport> airports);




#endif