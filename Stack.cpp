#ifndef STACK_CPP
#define STACK_CPP

#include "Stack.h"
#include <stdexcept>  // Include this to handle out_of_range exception

template <typename T>
void Stack<T>::push(const T& value) {
    data.push_back(value);
}

template <typename T>
void Stack<T>::pop() {
    if (!data.empty()) {
        data.pop_back();
    }
}

template <typename T>
T& Stack<T>::top() {
    if (!data.empty()) {
        return data.back();
    }
    // Handle empty stack case if needed
    throw std::out_of_range("Stack is empty");
}

template <typename T>
bool Stack<T>::empty() const {
    return data.empty();
}

template <typename T>
size_t Stack<T>::size() const {
    return data.size();  // Return the size of the underlying vector
}

#endif