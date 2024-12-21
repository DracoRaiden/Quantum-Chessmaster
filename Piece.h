#ifndef PIECE_H
#define PIECE_H

#include <iostream>
#include <vector>
#include <string>
#include <memory> // For smart pointers (optional but useful)
#include <stack>
#include "Board.h" // Make sure this is included
#include "Checkmate.h"
using namespace std;


//Give prototype implementation so that we can use all of its features in the Piece class
class Board;
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
    // Pure virtual function to get legal moves for a piece
    virtual vector<pair<int, int>> getLegalMoves(int startX, int startY, const Board &board) const = 0;
    // Pure virtual function to mark attacked squares
    virtual void markAttacks(int x, int y, Board &board) = 0;

    bool getColor() const { return isWhite; } // Returns the piece color
};

struct Move
{
    int startX, startY, endX, endY;

    // Constructor to initialize start and end positions
    Move(int startX, int startY, int endX, int endY)
        : startX(startX), startY(startY), endX(endX), endY(endY) {}

    // Default constructor (optional)
    Move() : startX(0), startY(0), endX(0), endY(0) {}
};

// King class
class King : public Piece
{
public:
    King(bool isWhite) : Piece(isWhite) {}
    char getSymbol() const override { return isWhite ? 'K' : 'k'; }
    bool isValidMove(int startX, int startY, int endX, int endY) const override;
    vector<pair<int, int>> getLegalMoves(int startX, int startY, const Board &board) const override;
    void markAttacks(int x, int y, Board& board) override;
};

// Queen class
class Queen : public Piece
{
public:
    Queen(bool isWhite) : Piece(isWhite) {}
    char getSymbol() const override { return isWhite ? 'Q' : 'q'; }
    bool isValidMove(int startX, int startY, int endX, int endY) const override;
    vector<std::pair<int, int>> getLegalMoves(int startX, int startY, const Board &board) const override;
    void markAttacks(int x, int y, Board& board) override;
};

// Rook class
class Rook : public Piece
{
public:
    Rook(bool isWhite) : Piece(isWhite) {}
    char getSymbol() const override { return isWhite ? 'R' : 'r'; }
    bool isValidMove(int startX, int startY, int endX, int endY) const override;
    vector<pair<int, int>> getLegalMoves(int startX, int startY, const Board &board) const override;
    void markAttacks(int x, int y, Board& board) override;
};

// Bishop class
class Bishop : public Piece
{
public:
    Bishop(bool isWhite) : Piece(isWhite) {}
    char getSymbol() const override { return isWhite ? 'B' : 'b'; }
    bool isValidMove(int startX, int startY, int endX, int endY) const override;
    vector<pair<int, int>> getLegalMoves(int startX, int startY, const Board &board) const override;
    void markAttacks(int x, int y, Board& board) override;
};

// Knight class
class Knight : public Piece
{
public:
    Knight(bool isWhite) : Piece(isWhite) {}
    char getSymbol() const override { return isWhite ? 'N' : 'n'; }
    bool isValidMove(int startX, int startY, int endX, int endY) const override;
    vector<std::pair<int, int>> getLegalMoves(int startX, int startY, const Board &board) const override;
    void markAttacks(int x, int y, Board& board) override;
};

// Pawn class
class Pawn : public Piece
{
public:
    Pawn(bool isWhite) : Piece(isWhite) {}
    char getSymbol() const override { return isWhite ? 'P' : 'p'; }
    bool isValidMove(int startX, int startY, int endX, int endY) const override;
    vector<pair<int, int>> getLegalMoves(int startX, int startY, const Board &board) const override;
    void markAttacks(int x, int y, Board &board) override;
};


#endif // PIECE_H