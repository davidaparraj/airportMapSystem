#include "queue.hpp"
#include <string>
#include <vector>
#include <fstream>
using namespace std;



// Function to create a queue
Queue* Queue::createQueue(int capacity) {
    Queue* queue = new Queue();
    queue->capacity = capacity;
    queue->size = 0;
    queue->front = queue->rear = -1;
    queue->array = new int[queue->capacity];
    return queue;
}

// Function to check if the queue is full
int Queue::isFull(Queue* queue) {
    return (queue->size == queue->capacity);
}

// Function to check if the queue is empty
int Queue::isEmpty(Queue* queue) {
    return (queue->size == 0);
}

// Function to add an element to the queue (enqueue)
void Queue::enqueue(Queue* queue, int item) {
    if (isFull(queue)) {
        cout << "Queue is full. Cannot enqueue " << item << ".\n";
        return;
    }

    // Circular increment for rear
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;

    // If it's the first element, set front to 0
    if (queue->front == -1) {
        queue->front = queue->rear;
    }

    queue->size = queue->size + 1;
    cout << item << " enqueued to queue\n";
}

// Function to remove an element from the queue (dequeue)
int Queue::dequeue(Queue* queue) {
    if (isEmpty(queue)) {
        cout << "Queue is empty. Cannot remove any elements." << endl;
        return -1;
    }

    int item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size--;
    
    return item;
}

// Function to get the front element
int Queue::getFront(Queue* queue) {
    if (isEmpty(queue)) {
        cout << "Queue is empty. No front element.\n";
        return -1;
    }
    return queue->array[queue->front];
}

// Function to get the rear element
int Queue::getRear(Queue* queue) {
    if (isEmpty(queue)) {
        cout << "Queue is empty. No rear element.\n";
        return -1;
    }
    return queue->array[queue->rear];
}