#include "stack.hpp"

#include <iostream>
#include <string>
using namespace std;

void Stack::Push(string input) {
    Node* temp = new Node();
    temp->str = input;
    temp->next = top;
    top = temp;

}
void Stack::Pop() {
    if(top == nullptr) {cout << "Stack Underflow" << endl;}
    string x = top->str;
    Node* temp = top;
    top = top->next;
    delete temp;
}

string Stack::Peek(){
    if(top == nullptr) {cout << "Empty Stack" << endl;}
    return (top->str);
}

bool Stack::Is_empty(){
    return (top == nullptr);
}

string Stack::backwards() {
    if(top == nullptr) {cout << "No string" << endl;}
    string x = top->str;
    
    if ()
    for (int i = 0; i < x.length(); i++) {
        
    }
}
