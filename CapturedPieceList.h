#ifndef CAPTUREDPIECELIST_H
#define CAPTUREDPIECELIST_H

#include <iostream>
#include <string>

// Structure for a node in the captured pieces list
struct CapturedPieceNode {
    std::string pieceType;
    bool isBlack; // True if black, false if white
    CapturedPieceNode* prev;
    CapturedPieceNode* next;

    // Constructor to initialize the node with piece type and color
    CapturedPieceNode(const std::string& type, bool color)
        : pieceType(type), isBlack(color), prev(nullptr), next(nullptr) {}
};

class CapturedPieceList {
public:
    CapturedPieceList();                      // Constructor
    ~CapturedPieceList();                     // Destructor
    void capturePiece(const std::string& pieceType, bool isBlack); // Capture a piece
    void restoreLastCapturedPiece();          // Restore the last captured piece
    void printCapturedPieces() const;         // Print captured pieces
    bool isCapturedPieceListEmpty() const;    // Check if the captured list is empty

private:
    CapturedPieceNode* blackHead;             // Head for black captured pieces list
    CapturedPieceNode* blackTail;             // Tail for black captured pieces list
    CapturedPieceNode* whiteHead;             // Head for white captured pieces list
    CapturedPieceNode* whiteTail;             // Tail for white captured pieces list
};

#endif // CAPTUREDPIECELIST_H