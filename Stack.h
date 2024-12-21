#ifndef STACK_H
#define STACK_H

#include <vector>

template <typename T>
class Stack {
public:
    // Push an element onto the stack
    void push(const T& value);

    // Pop an element from the stack
    void pop();

    // Return the top element of the stack
    T& top();

    // Check if the stack is empty
    bool empty() const;

    // Return the size of the stack
    size_t size() const;

private:
    std::vector<T> data;
};

#include "Stack.cpp"

#endif