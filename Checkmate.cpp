#include "checkmate.h"
#include "Board.h"
#include "AI.h"
#include "Piece.h"
#include <iostream>
#include <vector>

using namespace std;


class Board;

// Checkmate::Checkmate(Board &board, AI &ai) : board(board), ai(ai) {}

// bool Checkmate::isInCheck()
// {
//     // Get the position of the king for the current player
//     pair<int, int> kingPosition = getKingPosition(false); // Assuming AI plays as black
//     shared_ptr<Piece> kingPiece = board.getPiece(kingPosition.first, kingPosition.second);

//     // Check if the king is attacked by any opponent's pieces
//     for (int x = 0; x < 8; ++x)
//     {
//         for (int y = 0; y < 8; ++y)
//         {
//             shared_ptr<Piece> piece = board.getPiece(x, y);
//             if (piece != nullptr && piece->getColor() != kingPiece->getColor()) // Opponent's piece
//             {
//                 vector<pair<int, int>> legalMoves = piece->getLegalMoves(x, y, board);
//                 for (const auto &move : legalMoves)
//                 {
//                     if (isMoveAttackingKing({x, y}, move, false)) // Check if any move attacks the king
//                     {
//                         return true;
//                     }
//                 }
//             }
//         }
//     }
//     return false;
// }

// bool Checkmate::isMoveAttackingKing(pair<int, int> start, pair<int, int> end, bool isAIPlayer)
// {
//     // Get the king's position for the current player
//     pair<int, int> kingPosition = getKingPosition(isAIPlayer);

//     // Check if the move attacks the king's position
//     if (end == kingPosition)
//     {
//         return true;
//     }
//     return false;
// }

// pair<int, int> Checkmate::getKingPosition(bool isAIPlayer)
// {
//     // Loop through the board to find the king's position for the current player
//     for (int x = 0; x < 8; ++x)
//     {
//         for (int y = 0; y < 8; ++y)
//         {
//             shared_ptr<Piece> piece = board.getPiece(x, y);
//             if (piece != nullptr && piece->getColor() == isAIPlayer && piece->getSymbol() == 'K' || piece->getSymbol() == 'k')
//             {
//                 return {x, y};
//             }
//         }
//     }
//     throw runtime_error("King not found on the board!");
// }

// bool Checkmate::canKingBeSaved(bool isAIPlayer)
// {
//     // Get the position of the king for the current player
//     pair<int, int> kingPosition = getKingPosition(isAIPlayer);
//     shared_ptr<Piece> kingPiece = board.getPiece(kingPosition.first, kingPosition.second);

//     // Check if any legal move of the pieces can save the king
//     for (int x = 0; x < 8; ++x)
//     {
//         for (int y = 0; y < 8; ++y)
//         {
//             shared_ptr<Piece> piece = board.getPiece(x, y);
//             if (piece != nullptr && piece->getColor() == kingPiece->getColor())
//             {
//                 vector<pair<int, int>> legalMoves = piece->getLegalMoves(x, y, board);
//                 for (const auto &move : legalMoves)
//                 {
//                     // Try making the move without actually moving the piece
//                     if (board.movePiece(x, y, move.first, move.second))
//                     {
//                         // Check if this move would take the king out of check
//                         if (!isInCheck())
//                         {
//                             return true; // The king can be saved
//                         }
//                     }
//                 }
//             }
//         }
//     }
//     return false; // No move can save the king
// }

// bool Checkmate::isCheckmate()
// {
//     if (!isInCheck())
//     {
//         return false; // The king is not in check, so no checkmate is possible
//     }

//     // Check if the king can be saved
//     if (canKingBeSaved(false)) // Assuming AI plays as black
//     {
//         return false; // The king can still be saved, so it's not checkmate
//     }

//     return true; // The king is in check and cannot be saved, so it's checkmate
// }

// Default constructor
Square::Square() : x(0), y(0), piece('.'), isUnderAttack(false) {}

// Constructor with arguments
Square::Square(int x, int y, char piece) : x(x), y(y), piece(piece), isUnderAttack(false) {}


Checkmate::Checkmate(const vector<vector<shared_ptr<Piece>>>& board) {
    // Initialize the boardSquares to the size of the passed board
    // The board is 8x8, so we resize the boardSquares accordingly.
    boardSquares.resize(8, vector<shared_ptr<Piece>>(8, nullptr));

    // Copy the pieces from the input board to boardSquares
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            // Copy each piece from the input board into boardSquares
            if (board[i][j] != nullptr) {
                // If there's a piece, copy it over
                boardSquares[i][j] = board[i][j];
            }
        }
    }
}


bool Checkmate::isKingInCheck(int kingX, int kingY, Board &board) {
    // Loop through all pieces on the board

    cout << "Checking if King at (" << kingX << ", " << kingY << ") is in check." << endl;
    
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            shared_ptr<Piece> piece = boardSquares[x][y]; // Get the piece at the current square

            // Only consider opponent pieces
            if (piece != nullptr && piece->getColor() != boardSquares[kingX][kingY]->getColor()) {
                // Pass the Board reference to markAttacks()
                piece->markAttacks(x, y, board);  // Passing the Board reference

                // If the King's position is under attack, return true
                if (board.getSquare(kingX, kingY).isUnderAttack) {
                    return true;  // The King is in check
                }
            }
        }
    }

    // If no attacks were found, the King is not in check
    return false;  // The King is not in check
}

bool Checkmate::canKingEscape(int kingX, int kingY, Board &board) {
    // Check all adjacent squares around the King
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            // Skip the King's current position
            if (dx == 0 && dy == 0) continue;

            int newX = kingX + dx;
            int newY = kingY + dy;

            // Check if the new position is within bounds
            if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
                shared_ptr<Piece> pieceAtNewSquare = board.getPiece(newX, newY);

                // Ensure the King does not move to a square occupied by its own piece
                if (pieceAtNewSquare != nullptr && pieceAtNewSquare->getColor() == boardSquares[kingX][kingY]->getColor()) {
                    continue;  // Skip if there's a friendly piece
                }
                cout << "Checking escape at (" << newX << ", " << newY << ")" << endl;

                // Check if the square is under attack by checking the 'isUnderAttack' flag in the Square
                if (!board.getSquare(newX, newY).isUnderAttack) {
                    // If the King can move to this square and it's not under attack, return true (escape is possible)
                    return true;
                }
            }
        }
    }

    // If no valid escape squares found, return false (King can't escape)
    return false;
}

bool Checkmate::isCheckmate(int kingX, int kingY, Board &board) {
    // Step 1: Check if the King is in check
    if (!isKingInCheck(kingX, kingY, board)) {
        return false;  // If the King is not in check, it's not checkmate
    }

    // Step 2: Check if the King has any valid escape moves
    if (canKingEscape(kingX, kingY, board)) {
        return false;  // If the King can escape, it's not checkmate
    }

    // Step 3: If both conditions are met, it is checkmate
    return true;  // The King is in check and has no escape, so it's checkmate
}
