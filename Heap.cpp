#include "Heap.hpp"
#include <vector>

int MinHeap::parent_node(int i) {
    return (i-1) /2;
}

int MinHeap::left_node(int i) {
    return i*2 + 1;
}

int MinHeap::right_node(int i) {
    return i*2 + 2;
}

void MinHeap::HeapifyUp(int i) {
    while (i > 0 && data[i].distance < data[parent_node(i)].distance) {
        HeapNode temp = data[i];
        data[i] = data[parent_node(i)];
        data[parent_node(i)] = temp;
        i = parent_node(i);
    }  
}

void MinHeap::HeapifyDown(int i) {
int smallest = i;
    int left = left_node(i);
    int right = right_node(i);

    if (left < data.size() && data[left].distance < data[smallest].distance) {
        smallest = left;
    }
    if (right < data.size() && data[right].distance < data[smallest].distance) {
        smallest = right;
    }
    if (smallest != i) {
        HeapNode temp = data[i];
        data[i] = data[smallest];
        data[smallest]= temp;
        HeapifyDown(smallest);
    }
}

bool MinHeap::empty() {
    return data.empty();
}

void MinHeap::push(int distance, int node){
    data.push_back({distance, node});
    HeapifyUp(data.size()-1);   
}

HeapNode MinHeap::pop(){
    HeapNode top = data[0];
    data[0] = data.back();
    data.pop_back();
    if(!data.empty()){
        HeapifyDown(0);
    }
    return top;
}