#include "Board.h" // Make sure this is included
#include <memory>
#include <list>
#include <queue>
#include <stack>
#include <unordered_map>
#include <cstdlib>
#include <algorithm>

using namespace std;

bool Pawn::isValidMove(int startX, int startY, int endX, int endY) const
{
    int direction = isWhite ? -1 : 1; // White pawns move "up", black pawns move "down"

    // White pawn's single step forward
    if (isWhite && endX == startX + direction && endY == startY)
    {
        return true;
    }

    // Black pawn's single step forward
    if (!isWhite && endX == startX + direction && endY == startY)
    {
        return true;
    }

    // White pawn's double step forward (only from the starting row)
    if (isWhite && endX == startX + 2 * direction && endY == startY && startX == 6)
    {
        return true;
    }

    // Black pawn's double step forward (only from the starting row)
    if (!isWhite && endX == startX + 2 * direction && endY == startY && startX == 1)
    {
        return true;
    }

    // Diagonal capture (white or black)
    if (abs(startX - endX) == 1 && abs(startY - endY) == 1)
    {
        return true;
    }

    // En passant capture
    if (abs(startX - endX) == 1 && abs(startY - endY) == 1)
    {
        if (lastMove.isTwoSquareMove && lastMove.endX == startX && endY == lastMove.startY)
            return true;
    }

    return false;
}

bool Rook::isValidMove(int startX, int startY, int endX, int endY) const
{
    return (startX == endX || startY == endY); // Rook moves horizontally or vertically
}
bool Knight::isValidMove(int startX, int startY, int endX, int endY) const
{
    int dx = abs(endX - startX);
    int dy = abs(endY - startY);

    // Check if move follows "L" shape (1x2 or 2x1 move)
    if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2))
    {
        return true; // Valid knight move
    }

    return false; // Invalid move
}

bool Bishop::isValidMove(int startX, int startY, int endX, int endY) const
{
    return abs(endX - startX) == abs(endY - startY); // Diagonal moves
}

bool Queen::isValidMove(int startX, int startY, int endX, int endY) const
{
    return (startX == endX || startY == endY || abs(endX - startX) == abs(endY - startY));
}

bool King::isValidMove(int startX, int startY, int endX, int endY) const
{
    // Normal King movement: one square in any direction
    if (abs(endX - startX) <= 1 && abs(endY - startY) <= 1)
    {
        return true;
    }

    // Castling check: two-square horizontal move
    if (startX == endX && abs(endY - startY) == 2)
    { // Same row, two-square move
        // Ensure the king hasn't moved before
        if (!hasMoved)
        {
            return true; // Castling attempt is valid at this point
        }
    }

    return false; // Move is invalid if it doesn't satisfy the above conditions
}

// King piece movement logic
vector<pair<int, int>> King::getLegalMoves(int startX, int startY, const Board &board) const
{
    vector<std::pair<int, int>> moves;

    for (int dx : {-1, 0, 1})
    {
        for (int dy : {-1, 0, 1})
        {
            if (dx == 0 && dy == 0)
                continue;

            int newX = startX + dx, newY = startY + dy;
            if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8)
            {
                if (!board.isSquareOccupied(newX, newY) || board.getPiece(newX, newY)->getColor() != isWhite)
                {
                    moves.push_back({newX, newY});
                }
            }
        }
    }

    return moves;
}

void King::markAttacks(int x, int y, Board& board)
{
    // King can move one square in any direction
    vector<pair<int, int>> directions = {
        {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1} // All 8 directions
    };

    // Check each direction
    for (auto& dir : directions)
    {
        int targetX = x + dir.first, targetY = y + dir.second;

        // Check if the new position is out of bounds
        if (targetX >= 0 && targetX < 8 && targetY >= 0 && targetY < 8)
        {
            // If square is unoccupied or occupied by an enemy piece, mark as under attack
            if (!board.isSquareOccupied(targetX, targetY) || board.getPiece(targetX, targetY)->getColor() != isWhite)
            {
                board.getSquare(targetX, targetY).isUnderAttack = true;
            }
        }
    }
}


// Rook piece movement logic
vector<pair<int, int>> Rook::getLegalMoves(int startX, int startY, const Board &board) const
{
    vector<pair<int, int>> moves;

    // Rook can move horizontally or vertically
    vector<pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    for (auto &dir : directions)
    {
        int newX = startX, newY = startY;
        while (true)
        {
            newX += dir.first;
            newY += dir.second;

            if (newX < 0 || newX >= 8 || newY < 0 || newY >= 8)
                break;

            if (!board.isSquareOccupied(newX, newY))
            {
                moves.push_back({newX, newY});
            }
            else
            {
                if (board.getPiece(newX, newY)->getColor() != isWhite)
                {
                    moves.push_back({newX, newY});
                }
                break;
            }
        }
    }

    return moves;
}

void Rook::markAttacks(int x, int y, Board& board)
{
    // Rook can move horizontally or vertically
    vector<pair<int, int>> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1} // Horizontal and vertical movements
    };

    // Check each direction
    for (auto& dir : directions)
    {
        int targetX = x, targetY = y;

        // Move in the current direction
        while (true)
        {
            targetX += dir.first;
            targetY += dir.second;

            // Check if the new position is out of bounds
            if (targetX < 0 || targetX >= 8 || targetY < 0 || targetY >= 8)
                break;

            // If square is unoccupied or occupied by an enemy piece, mark as under attack
            if (!board.isSquareOccupied(targetX, targetY) || board.getPiece(targetX, targetY)->getColor() != isWhite)
            {
                board.getSquare(targetX, targetY).isUnderAttack = true;
            }

            // Stop if we hit a piece (occupied square)
            if (board.isSquareOccupied(targetX, targetY))
            {
                break;
            }
        }
    }
}


// Bishop piece movement logic
vector<pair<int, int>> Bishop::getLegalMoves(int startX, int startY, const Board &board) const
{
    std::vector<std::pair<int, int>> moves;

    // Bishop moves diagonally
    vector<pair<int, int>> directions = {{1, 1}, {-1, -1}, {1, -1}, {-1, 1}};
    for (auto &dir : directions)
    {
        int newX = startX, newY = startY;
        while (true)
        {
            newX += dir.first;
            newY += dir.second;

            if (newX < 0 || newX >= 8 || newY < 0 || newY >= 8)
                break;

            if (!board.isSquareOccupied(newX, newY))
            {
                moves.push_back({newX, newY});
            }
            else
            {
                if (board.getPiece(newX, newY)->getColor() != isWhite)
                {
                    moves.push_back({newX, newY});
                }
                break;
            }
        }
    }

    return moves;
}

void Bishop::markAttacks(int x, int y, Board& board)
{
    // Bishop moves diagonally in four directions
    vector<pair<int, int>> directions = {
        {1, 1}, {-1, -1}, {1, -1}, {-1, 1} // Diagonal movements
    };

    // Check each diagonal direction
    for (auto& dir : directions)
    {
        int targetX = x, targetY = y;

        // Move in the current direction
        while (true)
        {
            targetX += dir.first;
            targetY += dir.second;

            // Check if the new position is out of bounds
            if (targetX < 0 || targetX >= 8 || targetY < 0 || targetY >= 8)
                break;

            // If square is unoccupied or occupied by an enemy piece, mark as under attack
            if (!board.isSquareOccupied(targetX, targetY) || board.getPiece(targetX, targetY)->getColor() != isWhite)
            {
                board.getSquare(targetX, targetY).isUnderAttack = true;
            }

            // Stop if we hit a piece (occupied square)
            if (board.isSquareOccupied(targetX, targetY))
            {
                break;
            }
        }
    }
}


// Queen piece movement logic
vector<pair<int, int>> Queen::getLegalMoves(int startX, int startY, const Board &board) const
{
    std::vector<std::pair<int, int>> moves;

    // Queen can move like both a Rook and a Bishop
    vector<pair<int, int>> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}};

    for (auto &dir : directions)
    {
        int newX = startX, newY = startY;
        while (true)
        {
            newX += dir.first;
            newY += dir.second;

            if (newX < 0 || newX >= 8 || newY < 0 || newY >= 8)
                break;

            if (!board.isSquareOccupied(newX, newY))
            {
                moves.push_back({newX, newY});
            }
            else
            {
                if (board.getPiece(newX, newY)->getColor() != isWhite)
                {
                    moves.push_back({newX, newY});
                }
                break;
            }
        }
    }

    return moves;
}

void Queen::markAttacks(int x, int y, Board& board)
{
    // Queen can move like both a Rook and a Bishop
    vector<pair<int, int>> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}, // Rook-like movements
        {1, 1}, {-1, -1}, {1, -1}, {-1, 1} // Bishop-like movements
    };

    // Check each direction
    for (auto& dir : directions)
    {
        int targetX = x, targetY = y;
        
        // Move in the current direction
        while (true)
        {
            targetX += dir.first;
            targetY += dir.second;

            // Check if the new position is out of bounds
            if (targetX < 0 || targetX >= 8 || targetY < 0 || targetY >= 8)
                break;

            // If square is unoccupied or occupied by an enemy piece, mark as under attack
            if (!board.isSquareOccupied(targetX, targetY) || board.getPiece(targetX, targetY)->getColor() != isWhite)
            {
                board.getSquare(targetX, targetY).isUnderAttack = true;
            }

            // Stop if we hit a piece (occupied square)
            if (board.isSquareOccupied(targetX, targetY))
            {
                break;
            }
        }
    }
}


// Knight piece movement logic
vector<pair<int, int>> Knight::getLegalMoves(int startX, int startY, const Board &board) const
{
    vector<pair<int, int>> moves;

    // Knight moves in an 'L' shape
    vector<pair<int, int>> directions = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};

    for (auto &dir : directions)
    {
        int newX = startX + dir.first, newY = startY + dir.second;
        if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8)
        {
            if (!board.isSquareOccupied(newX, newY) || board.getPiece(newX, newY)->getColor() != isWhite)
            {
                moves.push_back({newX, newY});
            }
        }
    }

    return moves;
}

void Knight::markAttacks(int x, int y, Board& board)
{
    // Knight moves in an 'L' shape
    vector<pair<int, int>> directions = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, 
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
    };

    // Check each possible move direction
    for (auto& dir : directions)
    {
        int targetX = x + dir.first;
        int targetY = y + dir.second;

        // Check if the target position is within bounds
        if (targetX >= 0 && targetX < 8 && targetY >= 0 && targetY < 8)
        {
            // Mark the square as under attack if it's either empty or occupied by an enemy piece
            if (!board.isSquareOccupied(targetX, targetY) || board.getPiece(targetX, targetY)->getColor() != isWhite)
            {
                board.getSquare(targetX, targetY).isUnderAttack = true;
            }
        }
    }
}

// Pawn piece movement logic
vector<pair<int, int>> Pawn::getLegalMoves(int startX, int startY, const Board &board) const
{
    vector<pair<int, int>> moves;

    // Pawns move forward, but can attack diagonally
    int direction = (board.getPiece(startX, startY)->getColor() == 'w') ? -1 : 1; // White moves up, Black moves down

    // Move forward by one square
    if (!board.isSquareOccupied(startX + direction, startY))
    {
        moves.push_back({startX + direction, startY});
    }

    // Attack diagonally
    if (startY > 0 && board.isSquareOccupied(startX + direction, startY - 1) && board.getPiece(startX + direction, startY - 1)->getColor() != isWhite)
    {
        moves.push_back({startX + direction, startY - 1});
    }
    if (startY < 7 && board.isSquareOccupied(startX + direction, startY + 1) && board.getPiece(startX + direction, startY + 1)->getColor() != isWhite)
    {
        moves.push_back({startX + direction, startY + 1});
    }

    return moves;
}



// Pawn class method to mark attacks (non-const)
void Pawn::markAttacks(int x, int y, Board& board) {
    vector<pair<int, int>> attackMoves = getLegalMoves(x, y, board);

    for (const auto& move : attackMoves) {
        int targetX = move.first;
        int targetY = move.second;

        // Mark the square as under attack (no longer const-correct method)
        board.getSquare(targetX, targetY).isUnderAttack = true;
    }
}
