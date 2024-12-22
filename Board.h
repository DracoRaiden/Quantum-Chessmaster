#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include <string>
#include <memory> // For smart pointers (optional but useful)
// #include <stack>
#include "Piece.h"
#include "Checkmate.h"
#include "Stack.h"
#include "Queue.h"
#include "CapturedPieceList.h"



using namespace std;

class Piece;
class Square;
class Checkmate;


// Class representing the Chessboard
class Board
{
private:
    vector<vector<shared_ptr<Piece>>> board; // 2D vector of smart pointers to pieces
    // Stack to store history of board states (for undo functionality)
    Stack<vector<vector<shared_ptr<Piece>>>> history;
    Checkmate *checkmate; // Add Checkmate as a member of Board class

public:
    // New 2D vector of Squares
    vector<vector<Square>> squareBoard;
    Board(); // Constructor
    CapturedPieceList capturedPieces;

    shared_ptr<Piece> getPiece(int x, int y) const;
    void setupBoard();                                                  // Sets up initial board state
    void printBoard() const;                                            // Prints the board to the console
    bool isSquareOccupied(int x, int y) const;                          // Checks if a square is occupied
    bool isPathClear(int startX, int startY, int endX, int endY) const; // Checks if path is clear for non-knight moves
    // void buildAdjacencyList(vector<vector<int>>& adjList) const;
    bool movePiece(int startX, int startY, int endX, int endY); // Moves a piece
    void updateLastMove(int startX, int startY, int endX, int endY, bool isTwoSquareMove);
    void promotePawn(int x, int y);
    bool isSquareUnderAttack(int x, int y, bool color) const;
    bool canCastle(int startX, int startY, int endX, int endY) const;
    // Undo the last move
    void undoMove();
    bool redoMove();
    // Function to track the current state of the board and push it to the history stack
    void saveHistory();
    vector<pair<int, int>> getPossibleMoves(int startX, int startY) const;
    bool isRedoEmpty() const;
    int getHistorySize() const;
void resetAttackFlags();
    Square &getSquare(int x, int y);
    pair<int, int> getWhiteKingPosition();
    pair<int, int> getBlackKingPosition();
    // Function to return the board (access to the internal 2D vector of shared_ptr<Piece>)
    vector<vector<shared_ptr<Piece>>> getBoard() const;
    void capturePiece(const std::string& pieceType, bool isBlack);
    void restoreCapturedPiece();
    void printCapturedPieces() const;
    bool isKingInCheck(bool isWhite) const;
    // New method to convert coordinates to chessboard position
    string convertToPosition(int x, int y);
    bool isKingUnderAttack(int x, int y, bool byWhite) const;
    // bool isMoveRepeated(const Move &move);
    // void markMoveAsMade(const Move &move);
    // Move calculateAIMove();                          // Function to calculate AI's move
    // vector<Move> getLegalMovesForPlayer(int player); // Get all legal moves for a player
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

extern LastMove lastMove;

#endif // BOARD_H