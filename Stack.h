#ifndef STACK_H
#define STACK_H

#include <string>
#include <stdexcept>

class Stack {
public:
    Stack();
    ~Stack();

    void push(const std::string& value);
    void push(int value);
    void pop();
    std::string topString() const;  // Keep for compatibility with string
    int topInt() const;             // Keep for compatibility with int
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

    Node* topNode;
};

#endif // STACK_H
