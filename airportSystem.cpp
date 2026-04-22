#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "Graph.hpp"
#include "Heap.hpp"


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
    std::ifstream infile("airport.csv");
    if(!infile) {
        std::cerr << "Error. Could not open file.\n";
        return 1;
    }
    // Ignores first line (only contains headers)
    infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


    // Creates graph to store data from csv file
    Graph* system = new Graph;
    system->parseAndBuild(infile);


    return 0;
}