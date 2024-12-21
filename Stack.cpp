#include "Stack.h"

Stack::Stack() : topNode(nullptr) {}

Stack::~Stack() {
    while (!empty()) {
        pop();
    }
}

void Stack::push(const std::string& value) {
    Node* newNode = new Node(true, value);
    newNode->next = topNode;
    topNode = newNode;
}

void Stack::push(int value) {
    Node* newNode = new Node(false, value);
    newNode->next = topNode;
    topNode = newNode;
}

void Stack::pop() {
    if (empty()) {
        throw std::out_of_range("Stack is empty");
    }

    Node* temp = topNode;
    topNode = topNode->next;
    delete temp;
}

std::string Stack::topString() const {
    if (empty() || !topNode->isString) {
        throw std::out_of_range("Stack is empty or top is not a string");
    }
    return topNode->stringData;
}

int Stack::topInt() const {
    if (empty() || topNode->isString) {
        throw std::out_of_range("Stack is empty or top is not an int");
    }
    return topNode->intData;
}

bool Stack::empty() const {
    return topNode == nullptr;
}
