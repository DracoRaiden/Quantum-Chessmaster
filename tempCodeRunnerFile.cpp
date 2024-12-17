#include <iostream>
using namespace std;

// Node structure for the AVL Tree
struct AVLNode {
    int locID;
    int height;
    AVLNode *left;
    AVLNode *right;
    // Linked list for incidents at this location
    struct IncidentNode *incidentList;

    AVLNode(int locID) {
        this->locID = locID;
        this->height = 1;
        this->left = nullptr;
        this->right = nullptr;
        this->incidentList = nullptr;
    }
};

// Node structure for the incident linked list
struct IncidentNode {
    int incidentID;
    string description;
    IncidentNode *next;

    IncidentNode(int incidentID, string description) {
        this->incidentID = incidentID;
        this->description = description;
        this->next = nullptr;
    }
};

// Function to calculate the height of a node
int height(AVLNode *node) {
    if (node == nullptr) {
        return 0;
    }
    return node->height;
}

// Function to get the balance factor of a node
int getBalance(AVLNode *node) {
    if (node == nullptr) {
        return 0;
    }
    return height(node->left) - height(node->right);
}

// Function to update the height of a node
void updateHeight(AVLNode *node) {
    node->height = 1 + max(height(node->left), height(node->right));
}

// Function to perform a right rotation
AVLNode *rightRotate(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    updateHeight(y);
    updateHeight(x);

    // Return new root
    return x;
}

// Function to perform a left rotation
AVLNode *leftRotate(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    // Perform rotation
    y->left = x;
 x->right = T2;

    // Update heights
    updateHeight(x);
    updateHeight(y);

    // Return new root
    return y;
}

// Function to insert a new location into the AVL Tree
AVLNode* insert(AVLNode* node, int locID) {
    // Perform the normal BST insertion
    if (node == nullptr) {
        return new AVLNode(locID);
    }
    if (locID < node->locID) {
        node->left = insert(node->left, locID);
    } else if (locID > node->locID) {
        node->right = insert(node->right, locID);
    } else {
        // Duplicate locID are not allowed
        return node;
    }

    // Update the height of this ancestor node
    updateHeight(node);

    // Get the balance factor of this ancestor node to check whether
    // this node became unbalanced
    int balance = getBalance(node);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && locID < node->left->locID) {
        return rightRotate(node);
    }

    // Right Right Case
    if (balance < -1 && locID > node->right->locID) {
        return leftRotate(node);
    }

    // Left Right Case
    if (balance > 1 && locID > node->left->locID) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && locID < node->right->locID) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    // Return the (unchanged) node pointer
    return node;
}

// Function to print the AVL Tree in-order
void inOrder(AVLNode *root) {
    if (root != nullptr) {
        inOrder(root->left);
        cout << root->locID << " ";
        inOrder(root->right);
    }
}

int main() {
    AVLNode *root = nullptr;

    // Insert locations into the AVL Tree
    root = insert(root, 10);
    root = insert(root, 20);
    root = insert(root, 30);
    root = insert(root, 40);
    root = insert(root, 50);
    root = insert(root, 25);

    // Print the in-order traversal of the AVL Tree
    cout << "In-order traversal of the AVL tree is: ";
    inOrder(root);

    return 0;
}