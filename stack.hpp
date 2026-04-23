#ifndef STACK_HPP
#define STACK_HPP

#include <iostream>
#include <string>
using namespace std;

struct Node {
    string str;
    Node* next;
};

class Stack {
    Node* top;
public:
    
    Stack() {top = nullptr;};

    void Push(string input);
    void Pop();

    string Peek();

    bool Is_empty();


};

#endif