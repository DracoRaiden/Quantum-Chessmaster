#ifndef QUEUE_CPP
#define QUEUE_CPP

#include "Queue.h"
#include <stdexcept>  // Include this to handle out_of_range exception

template <typename T>
void Queue<T>::push(const T& value) {
    data.push_back(value);
}

template <typename T>
void Queue<T>::pop() {
    if (!data.empty()) {
        data.pop_front();
    }
}

template <typename T>
T& Queue<T>::front() {
    if (!data.empty()) {
        return data.front();
    }
    // Handle empty queue case if needed
    throw std::out_of_range("Queue is empty");
}

template <typename T>
bool Queue<T>::empty() const {
    return data.empty();
}

template <typename T>
size_t Queue<T>::size() const {
    return data.size();  // Return the size of the underlying deque
}


#endif