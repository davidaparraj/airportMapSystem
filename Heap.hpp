#ifndef HEAP_HPP
#define HEAP_HPP
#include <vector>


// Structure to record the combined weighted score given from the MinHeap.
struct HeapNode {
// the shortest path from source to node V (destination)
  int distance;
// the node that we are going to.
  int node;  
    
};

// self containing MinHeap that allows the Algorithm to pull info straight from the front of the vector.
class MinHeap {
    std::vector<HeapNode> data;
    // functions that use the vector to search for the child nodes to the left and right of the parent nodes
    // for searching the heap.
    int parent_node(int i);
    int left_node(int i);
    int right_node(int i);

    // HeapifyUp function to moves the nodes up the heap until it is balanced.
    void HeapifyUp(int i);
    // HeapifyDown function to used to initialize the Heap
    void HeapifyDown(int i);

public:

    bool empty();
    void push(int distance, int node);
    HeapNode pop();
};


#endif