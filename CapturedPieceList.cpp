#include "CapturedPieceList.h"

CapturedPieceList::CapturedPieceList() 
    : blackHead(nullptr), blackTail(nullptr), whiteHead(nullptr), whiteTail(nullptr) {}

CapturedPieceList::~CapturedPieceList() {
    // Clean up memory by deleting all nodes in the list for black and white pieces
    CapturedPieceNode* current = blackHead;
    while (current) {
        CapturedPieceNode* temp = current;
        current = current->next;
        delete temp;
    }

    current = whiteHead;
    while (current) {
        CapturedPieceNode* temp = current;
        current = current->next;
        delete temp;
    }
}

void CapturedPieceList::capturePiece(const std::string& pieceType, bool isBlack) {
    CapturedPieceNode* newPiece = new CapturedPieceNode(pieceType, isBlack);

    if (isBlack) {
        if (!blackTail) {  // If the list for black is empty
            blackHead = blackTail = newPiece;
        } else {
            blackTail->next = newPiece;
            newPiece->prev = blackTail;
            blackTail = newPiece;
        }
    } else {
        if (!whiteTail) {  // If the list for white is empty
            whiteHead = whiteTail = newPiece;
        } else {
            whiteTail->next = newPiece;
            newPiece->prev = whiteTail;
            whiteTail = newPiece;
        }
    }
}

void CapturedPieceList::restoreLastCapturedPiece() {
    if (!blackTail && !whiteTail) {
        std::cout << "No captured pieces to restore!" << std::endl;
        return;
    }

    // Restore the most recent captured piece (starting with black)
    if (blackTail) {
        CapturedPieceNode* lastCaptured = blackTail;
        std::cout << "Restoring piece: " << lastCaptured->pieceType << " (Black)" << std::endl;

        if (blackHead == blackTail) {  // If there's only one node for black
            delete blackHead;
            blackHead = blackTail = nullptr;
        } else {
            blackTail = blackTail->prev;
            delete lastCaptured;
            blackTail->next = nullptr;
        }
    }

    // If no black pieces remain, restore white pieces if any
    else if (whiteTail) {
        CapturedPieceNode* lastCaptured = whiteTail;
        std::cout << "Restoring piece: " << lastCaptured->pieceType << " (White)" << std::endl;

        if (whiteHead == whiteTail) {  // If there's only one node for white
            delete whiteHead;
            whiteHead = whiteTail = nullptr;
        } else {
            whiteTail = whiteTail->prev;
            delete lastCaptured;
            whiteTail->next = nullptr;
        }
    }
}

void CapturedPieceList::printCapturedPieces() const {
    if (!blackHead && !whiteHead) {
        std::cout << "Captured pieces:" << std::endl;
        std::cout << "No captured pieces!" << std::endl;
        return;
    }

    std::cout << "BLACK CAPTURED PIECES: ";
    CapturedPieceNode* current = blackHead;
    bool first = true;
    while (current) {
        if (!first) std::cout << ", ";
        std::cout << current->pieceType;
        current = current->next;
        first = false;
    }
    std::cout << std::endl;

    std::cout << "WHITE CAPTURED PIECES: ";
    current = whiteHead;
    first = true;
    while (current) {
        if (!first) std::cout << ", ";
        std::cout << current->pieceType;
        current = current->next;
        first = false;
    }
    std::cout << std::endl;
}

bool CapturedPieceList::isCapturedPieceListEmpty() const {
    return !blackHead && !whiteHead;
}
