#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include <string>
#include <memory> // For smart pointers (optional but useful)
#include <stack>
#include "Piece.h"

using namespace std;

// Class representing the Chessboard
class Board
{
private:
    vector<vector<shared_ptr<Piece>>> board; // 2D vector of smart pointers to pieces

    // Stack to store history of board states (for undo functionality)
    stack<vector<vector<shared_ptr<Piece>>>> history;

public:
    Board(); // Constructor
    shared_ptr<Piece> getPiece(int x, int y) const;
    void setupBoard();                                                  // Sets up initial board state
    void printBoard() const;                                            // Prints the board to the console
    bool isSquareOccupied(int x, int y) const;                          // Checks if a square is occupied
    bool isPathClear(int startX, int startY, int endX, int endY) const; // Checks if path is clear for non-knight moves
    bool movePiece(int startX, int startY, int endX, int endY);         // Moves a piece
    void updateLastMove(int startX, int startY, int endX, int endY, bool isTwoSquareMove);
    void promotePawn(int x, int y);
    bool isSquareUnderAttack(int x, int y, bool color) const;
    bool canCastle(int startX, int startY, int endX, int endY) const;
    // Undo the last move
    void undoMove();
    // Function to track the current state of the board and push it to the history stack
    void saveHistory();
    vector<pair<int, int>> getPossibleMoves(int startX, int startY) const;
    bool isMoveRepeated(const Move &move);
    void markMoveAsMade(const Move &move);
    Move calculateAIMove();                          // Function to calculate AI's move
    vector<Move> getLegalMovesForPlayer(int player); // Get all legal moves for a player
};

// Converts chess notation (e.g., "e2") to board indices
pair<int, int> convertToIndex(const string &position);

// Struct to track the last move made on the board
struct LastMove
{
    int startX, startY;
    int endX, endY;
    bool isTwoSquareMove;
    shared_ptr<Piece> pieceCaptured; // Change to shared_ptr<Piece>  Assuming Piece is the class or struct that represents a chess piece
};

#endif // BOARD_H
