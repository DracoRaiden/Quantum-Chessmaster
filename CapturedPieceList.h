#ifndef CAPTUREDPIECELIST_H
#define CAPTUREDPIECELIST_H

#include <iostream>
#include <string>

struct CapturedPieceNode {
    std::string pieceType;
    bool isBlack; // True if black, false if white
    std::string position;
    CapturedPieceNode* prev;
    CapturedPieceNode* next;

    CapturedPieceNode(const std::string& type, bool color, const std::string& pos)
        : pieceType(type), isBlack(color), position(pos), prev(nullptr), next(nullptr) {}
};

class CapturedPieceList {
public:
    CapturedPieceList();
    ~CapturedPieceList();
    void capturePiece(const std::string& pieceType, bool isBlack, const std::string& position);
    void restoreLastCapturedPiece();
    void printCapturedPieces() const;
    bool isCapturedPieceListEmpty() const;

private:
    CapturedPieceNode* head;
    CapturedPieceNode* tail;
};

#endif // CAPTUREDPIECELIST_H
