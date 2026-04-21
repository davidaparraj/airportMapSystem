#include "Graph.hpp"
#include "Heap.hpp"
#include <fstream>
#include <vector>
#include <string>
#include <iostream>



// TODO ----> FINISH LOGIC. Splits values and stores them in tokens[]
bool splitCSVLine(const std::string& line, std::string tokens[]) {
    int col = 0; // Represents current csv column that is being filled
    size_t i = 0; // Current character index

    return inQuotes;
}

// TODO (almost done)
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
std::vector<int> Graph::dijkstra(int source) {
    // Declaring Infinty as INF for the algo.
    const int INF = 1e9;
    std::vector<int> dist(airports.size(), INF);
    dist[source] = 0;

    MinHeap minHeap;
    minHeap.push(0, source);

    while(!minHeap.empty()) {
        HeapNode current = minHeap.pop();
        int currentScore = current.combined_score;
        int currentNode = current.node;

        if(currentScore > dist[currentNode]) continue;

        for (const Flight& flight : airports[currentNode].adjacent){
            int combined = flight.distance + flight.cost;
            int newDist =  dist[currentNode] + combined;

            if (newDist < dist[flight.destination]) {
                dist[flight.destination] = newDist;
                minHeap.push(newDist, flight.destination);
            }
        }
    }

    return dist;
}

