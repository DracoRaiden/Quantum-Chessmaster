#ifndef QUEUE_H
#define QUEUE_H

#include <deque>

template <typename T>
class Queue {
public:
    // Add an element to the queue
    void push(const T& value);

    // Remove an element from the queue
    void pop();

    // Get the front element of the queue
    T& front();

    // Check if the queue is empty
    bool empty() const;

    // Return the size of the stack
    size_t size() const;
    
private:
    std::deque<T> data;
};

#include "Queue.cpp"

#endif