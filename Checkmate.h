#ifndef CHECKMATE_H
#define CHECKMATE_H

#include "Board.h"
#include "AI.h"
#include "Piece.h"
#include <vector>

using namespace std;


class Board;
class Piece;

// class Checkmate
// {
// public:
//     // Constructor
//     Checkmate(Board &board, AI &ai);

//     // Method to check if the current player is in checkmate
//     bool isCheckmate();

//     // Method to check if the current player is in check
//     bool isInCheck();

// private:
//     // Reference to the board object
//     Board &board;

//     // Reference to the AI object
//     AI &ai;

//     // Helper method to check if a specific move attacks the king
//     bool isMoveAttackingKing(pair<int, int> start, pair<int, int> end, bool isAIPlayer);

//     // Helper method to get the position of the king
//     pair<int, int> getKingPosition(bool isAIPlayer);

//     // Helper method to check if any legal move can save the king
//     bool canKingBeSaved(bool isAIPlayer);
// };

// checkmate.h

// Define the Square class
class Square {
public:
    int x, y; // Position on the board
    bool isUnderAttack; // Whether this square is under attack
    char piece; // Store the piece (could be 'K' for King, 'Q' for Queen, etc.)
    
    Square(int x, int y, char piece = '.') : x(x), y(y), piece(piece), isUnderAttack(false) {}
};

class Checkmate {
public:
   // Constructor that accepts a reference to the board
    Checkmate(const vector<vector<shared_ptr<Piece>>>& board);

    bool isKingInCheck(int kingX, int kingY); // Check if the King is in check
    bool isCheckmate(int kingX, int kingY); // Check for checkmate

private:
    vector<vector<shared_ptr<Piece>>> boardSquares;  // Store the board in Checkmate

    //Moved funciton to the Piece class
    // void markAttacks();  // Function to mark all squares under attack

    bool canKingEscape(int kingX, int kingY);  // Check if the King can escape
};

#endif // CHECKMATE_H
