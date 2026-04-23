#ifndef QUEUE_HPP
#define QUEUE_HPP


#include <iostream>
using namespace std;

class Queue {
public:
    int front, rear, size;
    int capacity;
    int* array;

    int getRear(Queue* queue);
    int getFront(Queue* queue);
    int dequeue(Queue* queue);
    void enqueue(Queue* queue, int item);
    int isEmpty(Queue* queue);
    int isFull(Queue* queue);
    Queue* createQueue(int capacity);

};

#endif