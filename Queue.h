#ifndef QUEUE_H
#define QUEUE_H

#include <string>
#include <stdexcept>

class Queue {
public:
    Queue();
    ~Queue();

    void push(const std::string& value);
    void push(int value);
    void pop();
    std::string frontString() const;
    int frontInt() const;
    bool empty() const;

private:
    struct Node {
        bool isString;  // Flag to distinguish between string and int
        union {
            std::string stringData;
            int intData;
        };
        Node* next;
        Node(bool isStr, const std::string& strValue) : isString(isStr), stringData(strValue), next(nullptr) {}
        Node(bool isStr, int intValue) : isString(isStr), intData(intValue), next(nullptr) {}
        ~Node() {
            if (isString) {
                stringData.~basic_string();
            }
        }
    };

    Node* frontNode;
    Node* backNode;
};

#endif // QUEUE_H
