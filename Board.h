#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include <string>
#include <memory> // For smart pointers (optional but useful)
#include <stack>
#include <queue>
#include "CapturedPieceList.h" // Include CapturedPieceList header

using namespace std;

// Base class for all chess pieces
class Piece
{
protected:
    bool isWhite;  // true for white, false for black
    bool hasMoved; // Track if the piece has moved
public:
    Piece(bool isWhite) : isWhite(isWhite) {}
    // Determines if the piece belongs to the black player
    bool isBlack() const
    {
        return !isWhite;
    }
 // New method to get the type of the piece (e.g., "King", "Queen")
    virtual string getType() const = 0;  // Pure virtual function, must be implemented by derived classes

    bool getHasMoved() const { return hasMoved; }
    void setHasMoved(bool moved) { hasMoved = moved; }

    virtual char getSymbol() const = 0;                                             // Pure virtual function for symbol
    virtual bool isValidMove(int startX, int startY, int endX, int endY) const = 0; // Pure virtual function for move validation
    bool getColor() const { return isWhite; }                                       // Returns the piece color
};

// King class
class King : public Piece
{
public:
    King(bool isWhite) : Piece(isWhite) {}
    char getSymbol() const override { return isWhite ? 'K' : 'k'; }
    string getType() const override { return "King"; }
    bool isValidMove(int startX, int startY, int endX, int endY) const override;
};


// Queen class
class Queen : public Piece
{
public:
    Queen(bool isWhite) : Piece(isWhite) {}
    char getSymbol() const override { return isWhite ? 'Q' : 'q'; }
    string getType() const override { return "Queen"; }
    bool isValidMove(int startX, int startY, int endX, int endY) const override;
};


// Rook class
class Rook : public Piece
{
public:
    Rook(bool isWhite) : Piece(isWhite) {}
    char getSymbol() const override { return isWhite ? 'R' : 'r'; }
    string getType() const override { return "Rook"; }
    bool isValidMove(int startX, int startY, int endX, int endY) const override;
};


// Bishop class
class Bishop : public Piece
{
public:
    Bishop(bool isWhite) : Piece(isWhite) {}
    char getSymbol() const override { return isWhite ? 'B' : 'b'; }
    string getType() const override { return "Bishop"; }
    bool isValidMove(int startX, int startY, int endX, int endY) const override;
};


// Knight class
class Knight : public Piece
{
public:
    Knight(bool isWhite) : Piece(isWhite) {}
    char getSymbol() const override { return isWhite ? 'N' : 'n'; }
    string getType() const override { return "Knight"; }
    bool isValidMove(int startX, int startY, int endX, int endY) const override;
};


// Pawn class
class Pawn : public Piece
{
public:
    Pawn(bool isWhite) : Piece(isWhite) {}
    char getSymbol() const override { return isWhite ? 'P' : 'p'; }
    string getType() const override { return "Pawn"; }
    bool isValidMove(int startX, int startY, int endX, int endY) const override;
};


// Class representing the Chessboard
class Board
{
private:
    vector<vector<shared_ptr<Piece>>> board; // 2D vector of smart pointers to pieces

    // Stack to store history of board states (for undo functionality)
    stack<vector<vector<shared_ptr<Piece>>>> history;

public:
    queue<vector<vector<shared_ptr<Piece>>>> redoHistory;
    Board(); // Constructor
    CapturedPieceList capturedPieces;
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
    void redoMove();
    // Function to track the current state of the board and push it to the history stack
    void saveHistory();

    bool isKingInCheck(bool isWhite) const;         // Checks if the king of the given color is in check
 //   bool isCheckmate(bool isWhite);                // Checks if the player of the given color is in checkmate
 //   bool isStalemate(bool isWhite);                // Checks if the player of the given color is in stalemate
    bool hasLegalMoves(bool isWhite);              // Helper function to check if the player has any legal moves

    void capturePiece(const std::string& pieceType, bool isBlack);
    void restoreCapturedPiece();
    void printCapturedPieces() const;
    // New method to convert coordinates to chessboard position
    string convertToPosition(int x, int y);
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