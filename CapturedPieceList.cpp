#include "CapturedPieceList.h"

CapturedPieceList::CapturedPieceList() : head(nullptr), tail(nullptr) {}

CapturedPieceList::~CapturedPieceList() {
    CapturedPieceNode* current = head;
    while (current) {
        CapturedPieceNode* temp = current;
        current = current->next;
        delete temp;
    }
}

void CapturedPieceList::capturePiece(const std::string& pieceType, bool isBlack, const std::string& position) {
    CapturedPieceNode* newPiece = new CapturedPieceNode(pieceType, isBlack, position);
    if (!tail) {  // If the list is empty
        head = tail = newPiece;
    } else {
        tail->next = newPiece;
        newPiece->prev = tail;
        tail = newPiece;
    }
}

void CapturedPieceList::restoreLastCapturedPiece() {
    if (!tail) {
        std::cout << "No captured pieces to restore!" << std::endl;
        return;
    }

    // Get the last captured piece
    CapturedPieceNode* lastCaptured = tail;
    std::cout << "Restoring piece: " << lastCaptured->pieceType << " at " << lastCaptured->position << std::endl;

    // Restore it by removing it from the list (you can implement piece restoration logic here)
    if (head == tail) {  // If there's only one node
        delete head;
        head = tail = nullptr;
    } else {
        tail = tail->prev;
        delete lastCaptured;
        tail->next = nullptr;
    }
}

void CapturedPieceList::printCapturedPieces() const {
    if (!head) {
        std::cout << "No captured pieces!" << std::endl;
        return;
    }

    CapturedPieceNode* current = head;
    while (current) {
        std::cout << current->pieceType << " (" << (current->isBlack ? "Black" : "White") << ") captured at " << current->position << std::endl;
        current = current->next;
    }
}

bool CapturedPieceList::isCapturedPieceListEmpty() const {
    return head == nullptr;
}
