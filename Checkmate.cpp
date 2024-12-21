#include "checkmate.h"
#include "Board.h"
#include "AI.h"
#include "Piece.h"
#include <iostream>
#include <vector>

using namespace std;

Checkmate::Checkmate(Board &board, AI &ai) : board(board), ai(ai) {}

bool Checkmate::isInCheck()
{
    // Get the position of the king for the current player
    pair<int, int> kingPosition = getKingPosition(false); // Assuming AI plays as black
    shared_ptr<Piece> kingPiece = board.getPiece(kingPosition.first, kingPosition.second);

    // Check if the king is attacked by any opponent's pieces
    for (int x = 0; x < 8; ++x)
    {
        for (int y = 0; y < 8; ++y)
        {
            shared_ptr<Piece> piece = board.getPiece(x, y);
            if (piece != nullptr && piece->getColor() != kingPiece->getColor()) // Opponent's piece
            {
                vector<pair<int, int>> legalMoves = piece->getLegalMoves(x, y, board);
                for (const auto &move : legalMoves)
                {
                    if (isMoveAttackingKing({x, y}, move, false)) // Check if any move attacks the king
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Checkmate::isMoveAttackingKing(pair<int, int> start, pair<int, int> end, bool isAIPlayer)
{
    // Get the king's position for the current player
    pair<int, int> kingPosition = getKingPosition(isAIPlayer);

    // Check if the move attacks the king's position
    if (end == kingPosition)
    {
        return true;
    }
    return false;
}

pair<int, int> Checkmate::getKingPosition(bool isAIPlayer)
{
    // Loop through the board to find the king's position for the current player
    for (int x = 0; x < 8; ++x)
    {
        for (int y = 0; y < 8; ++y)
        {
            shared_ptr<Piece> piece = board.getPiece(x, y);
            if (piece != nullptr && piece->getColor() == isAIPlayer && piece->getSymbol() == 'K' || piece->getSymbol() == 'k')
            {
                return {x, y};
            }
        }
    }
    throw runtime_error("King not found on the board!");
}

bool Checkmate::canKingBeSaved(bool isAIPlayer)
{
    // Get the position of the king for the current player
    pair<int, int> kingPosition = getKingPosition(isAIPlayer);
    shared_ptr<Piece> kingPiece = board.getPiece(kingPosition.first, kingPosition.second);

    // Check if any legal move of the pieces can save the king
    for (int x = 0; x < 8; ++x)
    {
        for (int y = 0; y < 8; ++y)
        {
            shared_ptr<Piece> piece = board.getPiece(x, y);
            if (piece != nullptr && piece->getColor() == kingPiece->getColor())
            {
                vector<pair<int, int>> legalMoves = piece->getLegalMoves(x, y, board);
                for (const auto &move : legalMoves)
                {
                    // Try making the move without actually moving the piece
                    if (board.movePiece(x, y, move.first, move.second))
                    {
                        // Check if this move would take the king out of check
                        if (!isInCheck())
                        {
                            return true; // The king can be saved
                        }
                    }
                }
            }
        }
    }
    return false; // No move can save the king
}

bool Checkmate::isCheckmate()
{
    if (!isInCheck())
    {
        return false; // The king is not in check, so no checkmate is possible
    }

    // Check if the king can be saved
    if (canKingBeSaved(false)) // Assuming AI plays as black
    {
        return false; // The king can still be saved, so it's not checkmate
    }

    return true; // The king is in check and cannot be saved, so it's checkmate
}
