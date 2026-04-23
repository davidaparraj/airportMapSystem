#include "Graph.hpp"
#include "Heap.hpp"
#include "UnionFind.hpp"
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>



/* ---------- Helper functions ---------- */
bool splitCSVLine(const std::string& line, std::string tokens[]) {
    int col = 0; // Represents current csv column that is being filled
    size_t i = 0; // Current character index

    // Get values from every column of the line and store them in tokens respectively
    while(i < line.size() && col < 6) {
        if(line[i] == '"') {
            i++; // skips opening quote
            while(i < line.size() && line[i] != '"') {
                tokens[col] += line[i];
                i++; // moves to next character
            }
            i++; // Skips closing quote
            i++; // skips following comma
        }
        else {
            while(i < line.size() && line[i] != ',') {
                tokens[col] += line[i];
                i++;
            }
            i++; // skips following comma
        }
        col++; // moves to fill next column data
    }
    return col == 6; // returns true when all columns where filled for all lines
}

std::string getState(const std::string& city) {
    // Return last two characters excluding the closing quote
    // Ex: FL" ----> only return FL
    std::string state;

    state += city[city.size() - 2];
    state += city[city.size() - 1];
    return state;
}

int getTotalCost(const std::vector<int> path, const std::vector<Airport> airports) {
    // Calculate total cost by walking the path forward
    int totalCost = 0;
    for (int i = path.size()-1; i > 0; i--) {
        int from = path[i];
        int to = path[i - 1];

        for(const Flight flight : airports[from].adjacent) {
            if(flight.destination == to) {
                totalCost += flight.cost;
                 break;
            }
        }
    }
    return totalCost;
}


void Graph::parseAndBuild(std::ifstream& infile) {
    // Example: ABE,DTW,"Allentown, PA","Detroit, MI",424,374
    std::string line;

    // Iterate through every row from csv file
    while(std::getline(infile, line)) {
        // TODO: Separate values with ",". Consider city is in format "City, STATE".
        // Then parse it

        if(line.empty() || line == "\r") continue;
        std::string tokens[6]; // Stores data from current line and organizes it for splitCSVLine
        if(!splitCSVLine(line, tokens)) continue;

        // Elements from tokens[] are used to add code and city info to nodes later on
        std::string originCode = tokens[0];
        std::string destCode =  tokens[1];
        std::string originCity =  tokens[2];
        std::string destCity =  tokens[3];
        int distance =  std::stoi(tokens[4]);
        int cost =  std::stoi(tokens[5]);

        // Add origin and destination for optimization
        addNode(originCode, originCity);
        addNode(destCode, destCity);
        addEdge(originCode, destCode, distance, cost);
    }
}

int Graph::findNode(const std::string& code) const {
    for(int i = 0; i < airports.size(); i++) {
        if(airports[i].code == code) {
            return i;
        }
    }

    return -1; // Airport does not exist
}

void Graph::addNode(const std::string& code, const std::string& city) {
    int index = findNode(code);

    if(index != -1) {return;}

    airports.push_back(Airport(code, city));
}

void Graph::addEdge(const std::string& from, const std::string& to, int distance, int cost) {
    int fromIndex = findNode(from);
    int toIndex = findNode(to);

    if(fromIndex == -1 || toIndex == -1) {
        std::cerr << "Warning: edge references unknown airport (" << from << " -> "<< to << ")\n";
        return;
    }

    airports[fromIndex].adjacent.push_back(Flight(toIndex, distance, cost));
}

// Shortest Possible Path algorith (Dijkstra) 
// Finds all of the Paths and adds them into the MinHeap
// Minheap sorts for shorest path staying at the root
// Dijkstra ends by pulling the root from the MinHeap
void Graph::dijkstra(int source, std::vector<int>& prev, std::vector<int>& dist) {
    // Declaring Infinty as INF for the algo
    const int INF = 1e9;
    dist.assign(airports.size(), INF);
    prev.assign(airports.size(), -1);
    dist[source] = 0; // Set source node to zero

    // Create heap and add root
    MinHeap minHeap;
    minHeap.push(0, source);
    

    while(!minHeap.empty()) {
        HeapNode current = minHeap.pop();
        int currentDistance = current.distance;
        int currentNode = current.node;

        // Current distance must be less than distance to currentNode
        if(currentDistance > dist[currentNode]) continue; // Not fastest path. Skip it

        for (const Flight& flight : airports[currentNode].adjacent){
            int newDist =  dist[currentNode] + flight.distance;

            // Update distance if shorter path is found
            if (newDist < dist[flight.destination]) {
            dist[flight.destination] = newDist;
            prev[flight.destination] = currentNode;
            minHeap.push(newDist, flight.destination); // push updated distance and airport index
            }
        }
    }
}

// Task 2) optimizes shortest path according to distance. Report distance and cost at the end
void Graph::printShortestPath(const std::string& origin, const std::string& dest){
    const int INF = 1e9;
    int src = findNode(origin);
    int dst = findNode(dest);

    std::cout << "Shortest route from " << origin << " to " << dest << ": ";

    if (src == -1 || dst == -1) { // Path doesn't exist
        std::cout << "None" << std::endl;
        return; 
    }

    std::vector<int> prev;
    std::vector<int> dist;
    dijkstra(src, prev, dist); // Finds shortest paths
    
    if (dist[dst] == INF) { // Path not found
        std::cout << "None" << std::endl;
        return;
    }

    // Reconstruct path by walking backwards
    std::vector<int> path;
    for (int at = dst; at != -1; at = prev[at]) {
        path.push_back(at);
    }

    // Print shortest path
    for (int i = path.size()-1; i >= 0; i--) {
        std::cout << airports[path[i]].code;
        if (i != 0) {
            std::cout << "->";
        }
    }

    int totalCost = getTotalCost(path, airports);
    //dist[dst] = distance calculated in Dijsktra's algorithm (dist[flight.destination])
    std::cout << ". The length is " << dist[dst] << ". The cost is " << totalCost << std::endl;
}

// Task 3) optimizes shortest path from origin to all airports in one state. Prints path, distance and cost
// TODO ----> FIX FORMATTING
void Graph::printShortestPathBySate(const std::string& origin, const std::string& state) {
    // Run Dijktra from origin
    const int INF = 1e9;
    std::cout << "Shortest paths from " << origin << " to " << state << " state airports are: ";

    int src = findNode(origin);
    if(src == -1) { // source not found.
        std::cout << "None.\n";
        return;
    }

    std::vector<int> prev;
    std::vector<int> dist;
    dijkstra(src, prev, dist); // Find shortest paths

    // Print header
    std::cout << "\n";
    std::cout   << std::left 
                << std::setw(30) << "Path"
                << std::setw(15) << "Length"
                << "Cost\n";

    // Find all codes for given state
    // Loop through every airport in the graph
    // For each airport, check if its state matches the requested state
    // If it does and a path exists, print i(path to destination)
    for(int i = 0; i < airports.size(); i++) {
        std::string expState = getState(airports[i].city);
        if(expState != state) continue; // Skips if city is not in the state

        if(dist[i] == INF) { // Couldn't find path to airport
            continue; // Skip to next airport
        }

        // Reconstruct path by walking backwards
        std::vector<int> path;
        for (int at = i; at != -1; at = prev[at]) {
            path.push_back(at);
        }
        // Print shortest path
        std::string pathStr;
        for (int j = path.size()-1; j >= 0; j--) {
            pathStr += airports[path[j]].code;
            if(j != 0) {
                pathStr += "->";
            }
        }
        
        // Print length
        std::cout   << std::left 
                    << std::setw(30) << pathStr
                    << std::setw(15) << dist[i]
                    << getTotalCost(path, airports) << "\n";
    }
}

// Finds the shortest path with a designated amount of stops
// -- tracks stops left, total cost of path, whether an aiport has been visited to not do repeats, and chooses the shortest path from the ones that succeed
void Graph::findPathWithStops(int current, int dest, int stopsLeft, int currDist, int currCost, std::vector<int>& path, std::vector<bool>& visited, std::vector<int>& bestPath, int& bestDist, int& bestCost){
    // Checks if we are at the destination with 0 stops left
    if (stopsLeft == -1 && current == dest) {
        // makes a value for the combined cost and distance for the trip
        int combined = currCost + currDist;
        // checks if the current combined weight is smaller than the already check best combined
        if (combined < bestCost + bestDist) {
            // if the current path is the best found then it changes the variables to represent it
            bestDist = currDist;
            bestCost = currCost;
            bestPath = path;
        }
        return;
    }

    // no stops left but not at the destination find new route
    if (stopsLeft == -1) { return; }

    // for loop that checks the airports adjacent airports and the flights to them
    for (const Flight& flight : airports[current].adjacent) {
        // if the airport hasnt been visited yet, continue to the rest
        if (!visited[flight.destination]) {
            visited[flight.destination] = true; // mark the node as visited
            path.push_back(flight.destination); // push the airport to the path

            // recursive function to find the best path
            findPathWithStops(flight.destination, dest, stopsLeft-1, currDist + flight.distance, currCost + flight.cost, path, visited, bestPath, bestDist, bestCost);

            // back track through the path
            path.pop_back();
            visited[flight.destination] = false;
        }
    }
}



void Graph::printPathWithStops(const std::string& origin, const std::string& dest, int stops) {
    int src = findNode(origin);
    int dst = findNode(dest);

    if (src == -1 || dst == -1) {
        std::cout << "\nShortest route from " << origin << " to " << dest << " with " << stops << " stops: None" << std::endl;
        return;
    }

    const int INF = 1e9; // defining infinity
    std::vector<int> path; 
    std::vector<int> bestPath;
    std::vector<bool> visited(airports.size(), false); // creating the visited vector 
    int bestDist = INF;
    int bestCost = INF;

    // Mark source as visited to prevent revisiting
    visited[src] = true;
    path.push_back(src);

    // calling the recursive helper function of findPathWithStops 
    findPathWithStops(src, dst, stops, 0, 0, path, visited, bestPath, bestDist, bestCost);

    // if a path doesnt exist with that amount of stops it prints none
    if (bestPath.empty()) {
        std::cout << "\nShortest route from " << origin << " to " << dest << " with " << stops << " stops: None" << std::endl;
        return;
    }

    // prints the shortest route with the designated amount of stops
    std::cout << "\nShortest route from " << origin << " to " << dest << " with " << stops << " stops: ";
    // for loop printing the airports that were added to the array for the most efficient path.
    for (int i = 0; i < bestPath.size(); i++) {
        std::cout << airports[bestPath[i]].code;
        if (i != bestPath.size() - 1) std::cout << " -> ";
    }
    std::cout << ". The length is " << bestDist << ". The cost is " << bestCost << std::endl;
}


// Task 5 printing the amount of connections -- inbound + outbound flights -- an airport has
void Graph::printConnectionCounts() {
    // Count inbound connections
    std::vector<int> inbound(airports.size(), 0);
    for (int i = 0; i < airports.size(); i++) {
        for (const Flight& flight : airports[i].adjacent) {
            inbound[flight.destination]++;
        }
    }

    // Push total connections into  the Minheap
    MinHeap minHeap;
    for (int i = 0; i < airports.size(); i++) {
        int outbound = airports[i].adjacent.size();
        int total = inbound[i] + outbound;
        minHeap.push(total, i);
    }

    // Pop everything into temp vector
    std::vector<HeapNode> sorted;
    while (!minHeap.empty()) {
        sorted.push_back(minHeap.pop());
    }

    // Print in reverse for highest to lowest
    std::cout << "Airport\t\tConnections\n";
    for (int i = sorted.size() - 1; i >= 0; i--) {
        std::cout << airports[sorted[i].node].code << "\t\t" << sorted[i].distance << "\n";
    }
}

/* Task 6) 
    Creates an undirected Graph.
    Only stores one directed edge for each pair of nodes u an v (either (u,v) or (v,u), not both).
    We keep that single edge with its cost as an undirected weighted edge. Distance is ignored
*/

int Graph::getEdgeCost(int u, int v) const {
    for (int i = 0; i < airports[u].adjacent.size(); i++) {
        if (airports[u].adjacent[i].destination == v)
            return airports[u].adjacent[i].cost;
    }

    return -1; // Edge doesn't exist
}

Graph Graph::buildUndirected() const {
    Graph newGraph;
    int n = airports.size();

    // Copy all nodes first
    for(int i = 0; i < n; i++) {
        newGraph.addNode(airports[i].code, airports[i].city);
    }
    
    // Determine how to add the undirected edges by cost
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < airports[i].adjacent.size(); j++) {
            // Check adjacent to ssee if reverse edge was visited
            int dest = airports[i].adjacent[j].destination;
            if(dest <= i) continue; // Skip. Pair already visited or to be visited

            int forwardCost = airports[i].adjacent[j].cost; // Cost from u to v
            int reverseCost = getEdgeCost(dest, i); // Cost from v to u

            int costToUse;
            if(reverseCost == -1) {
                // There is only one direction
                costToUse = forwardCost;
            }
            else {
                // Both directions exist. Keep the lowest in code
                costToUse = (forwardCost < reverseCost) ? forwardCost: reverseCost;
            }

            // Add node and undirected edge to build new graph
            newGraph.addEdge(airports[i].code, airports[dest].code, 0, costToUse);
            newGraph.addEdge(airports[dest].code, airports[i].code, 0, costToUse);
        }
    }
    return newGraph;
}


// Task 7 Utilizing Prims for making a Minimal Spanning Tree.

void Graph::primMST() {
    const int INF = 1e9; // defining infinity
    int n = airports.size(); // definin n as the total number of aiport nodes

    // Check connectivity using BFS first
    std::vector<int> visited = BFS(0);
    if (visited.size() != n) { // if the size of the visited vector is not equal to the total # of airports
        std::cout << "MST cannot be formed: graph is disconnected." << std::endl;
        return;
    }

    std::vector<int> key(n, INF);      // minimum cost to reach each node
    std::vector<int> parent(n, -1);    // tracks MST edges
    std::vector<bool> inMST(n, false); // tracks if node is in MST

    key[0] = 0; // start from node 0
    int totalCost = 0; // set totalcost to 0

    for (int count = 0; count < n; count++) { // checks amount of times taht there are aiports
        // Find minimum key node not yet in MST 
        int u = -1;
        for (int i = 0; i < n; i++) {
            if (!inMST[i] && (u == -1 || key[i] < key[u])) { // if not in the MST and keynode not in and key i < key u
                u = i;
            }
        }

        inMST[u] = true; // changes boolean to true for the node being in the MST
        totalCost += key[u];

        // Update keys of adjacent nodes
        for (const Flight& flight : airports[u].adjacent) {
            int v = flight.destination;
            if (!inMST[v] && flight.cost < key[v]) {
                key[v] = flight.cost;
                parent[v] = u;
            }
        }
    }

    // Print MST
    std::cout << "Minimal Spanning Tree (Prim's):\n";
    std::cout << std::left << std::setw(20) << "Edge" << "Weight\n";
    for (int i = 1; i < n; i++) {
        std::cout << std::left << std::setw(20)
                  << airports[parent[i]].code + " - " + airports[i].code
                  << key[i] << "\n";
    }
    std::cout << "Total Cost of MST: " << totalCost << std::endl;
}