#ifndef CHECKMATE_H
#define CHECKMATE_H

#include "Board.h"
#include "AI.h"
#include "Piece.h"

class Checkmate
{
public:
    // Constructor
    Checkmate(Board &board, AI &ai);

    // Method to check if the current player is in checkmate
    bool isCheckmate();

    // Method to check if the current player is in check
    bool isInCheck();

private:
    // Reference to the board object
    Board &board;

    // Reference to the AI object
    AI &ai;

    // Helper method to check if a specific move attacks the king
    bool isMoveAttackingKing(pair<int, int> start, pair<int, int> end, bool isAIPlayer);

    // Helper method to get the position of the king
    pair<int, int> getKingPosition(bool isAIPlayer);

    // Helper method to check if any legal move can save the king
    bool canKingBeSaved(bool isAIPlayer);
};

#endif // CHECKMATE_H
