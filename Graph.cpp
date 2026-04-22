#include "Graph.hpp"
#include "Heap.hpp"
#include <fstream>
#include <vector>
#include <string>
#include <iostream>




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

// Shortest Possible Path algorith (Dijksra) 
// Finds all of the Paths and adds them into the MinHeap
// Minheap sorts for shorest path staying at the root
// Dijkstra ends by pulling the root from the MinHeap
std::vector<int> Graph::dijkstra(int source, std::vector<int>& prev, std::vector<int>& costDist) {
    // Declaring Infinty as INF for the algo.
    const int INF = 1e9;
    std::vector<int> dist(airports.size(), INF);
    costDist.assign(airports.size(), INF);
    prev.assign(airports.size(), -1);
    dist[source] = 0;

    MinHeap minHeap;
    minHeap.push(0, source);

    while(!minHeap.empty()) {
        HeapNode current = minHeap.pop();
        int currentScore = current.combined_score;
        int currentNode = current.node;

        if(currentScore > dist[currentNode] + costDist[currentNode]) continue;

        for (const Flight& flight : airports[currentNode].adjacent){
            int newDist =  dist[currentNode] + flight.distance;
            int newCost = costDist[currentNode] + flight.cost;
            int combined = newDist + newCost;
  

            if (combined < dist[currentNode] + costDist[currentNode] + flight.distance + flight.cost) {
                dist[flight.destination] = newDist;
                costDist[flight.destination] = newCost;
                prev[flight.destination] = currentNode;
                minHeap.push(combined, flight.destination);
            }
        }
    }

    return dist;
}

void Graph::printShortestPath(const std::string& origin, const std::string& dest){
    int src = findNode(origin);
    int dst = findNode(dest);

    if (src == -1 || dst == -1) {
        std::cout << "Shortest route from " << origin << " to " << dest << " : None" << std::endl;
        return; 
    }

    std::vector<int> prev;
    std::vector<int> costDist;
    std::vector<int> dist = dijkstra(src, prev, costDist);  

    if (dist[dst] == 1e9) {
        std::cout << "Shortest route from " << origin << " to " << dest << " : None" << std::endl;
        return;
    }

    std::vector<int> path;
    for (int at = dst; at != -1; at = prev[at]) {
        path.push_back(at);
    }

    std::cout << "Shortest route from " << origin << " to " << dest << ": ";
    for (int i = path.size()-1; i >= 0; i--) {
        std::cout << airports[path[i]].code;
        if (i != 0) {
            std::cout << " -> ";
        }
    }
    std::cout << ". The length is " << dist[dst] << ". The cost is " << costDist[dst] << std::endl;
}

