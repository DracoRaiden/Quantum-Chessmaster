#include "Queue.h"

Queue::Queue() : frontNode(nullptr), backNode(nullptr) {}

Queue::~Queue() {
    while (!empty()) {
        pop();
    }
}

void Queue::push(const std::string& value) {
    Node* newNode = new Node(true, value);
    if (empty()) {
        frontNode = backNode = newNode;
    } else {
        backNode->next = newNode;
        backNode = newNode;
    }
}

void Queue::push(int value) {
    Node* newNode = new Node(false, value);
    if (empty()) {
        frontNode = backNode = newNode;
    } else {
        backNode->next = newNode;
        backNode = newNode;
    }
}

void Queue::pop() {
    if (empty()) {
        throw std::out_of_range("Queue is empty");
    }

    Node* temp = frontNode;
    frontNode = frontNode->next;
    if (frontNode == nullptr) {
        backNode = nullptr;
    }
    delete temp;
}

std::string Queue::frontString() const {
    if (empty() || !frontNode->isString) {
        throw std::out_of_range("Queue is empty or front is not a string");
    }
    return frontNode->stringData;
}

int Queue::frontInt() const {
    if (empty() || frontNode->isString) {
        throw std::out_of_range("Queue is empty or front is not an int");
    }
    return frontNode->intData;
}

bool Queue::empty() const {
    return frontNode == nullptr;
}
