#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include <string>
#include <memory> // For smart pointers (optional but useful)
#include <stack>

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
    bool isValidMove(int startX, int startY, int endX, int endY) const override;
};

// Queen class
class Queen : public Piece
{
public:
    Queen(bool isWhite) : Piece(isWhite) {}
    char getSymbol() const override { return isWhite ? 'Q' : 'q'; }
    bool isValidMove(int startX, int startY, int endX, int endY) const override;
};

// Rook class
class Rook : public Piece
{
public:
    Rook(bool isWhite) : Piece(isWhite) {}
    char getSymbol() const override { return isWhite ? 'R' : 'r'; }
    bool isValidMove(int startX, int startY, int endX, int endY) const override;
};

// Bishop class
class Bishop : public Piece
{
public:
    Bishop(bool isWhite) : Piece(isWhite) {}
    char getSymbol() const override { return isWhite ? 'B' : 'b'; }
    bool isValidMove(int startX, int startY, int endX, int endY) const override;
};

// Knight class
class Knight : public Piece
{
public:
    Knight(bool isWhite) : Piece(isWhite) {}
    char getSymbol() const override { return isWhite ? 'N' : 'n'; }
    bool isValidMove(int startX, int startY, int endX, int endY) const override;
};

// Pawn class
class Pawn : public Piece
{
public:
    Pawn(bool isWhite) : Piece(isWhite) {}
    char getSymbol() const override { return isWhite ? 'P' : 'p'; }
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
    void undoMove();
    void saveHistory();   //to get past input history

    bool isValidMoveUnderCheck(int startX, int startY, int endX, int endY, bool isWhite);
    bool makeMove(int startX, int startY, int endX, int endY, bool isWhite);
    bool isKingInCheck(bool isWhite) const;         // Checks if the king of the given color is in check
    bool isCheckmate(bool isWhite);                // Checks if the player of the given color is in checkmate
    bool isStalemate(bool isWhite);                // Checks if the player of the given color is in stalemate
    bool hasLegalMoves(bool isWhite);              // Helper function to check if the player has any legal moves

    // Check if any piece can move to protect the king.
    bool canMoveToProtectKing(bool isWhite);



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