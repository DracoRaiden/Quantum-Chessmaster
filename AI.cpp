#include "AI.h"
#include "Board.h"
#include <cstdlib>  // For random number generation
#include <ctime>    // For seeding the random number generator
#include <iostream>

using namespace std;

// Constructor that initializes the AI with the player's color and the game board
AI::AI(Board& b, int playerColor) : board(b), playerColor(playerColor) {
    // Seed the random number generator to get different results each time
    srand(time(nullptr));
    
}

vector<Move> AI::getLegalMovesForPlayer(int color) {
    vector<Move> legalMoves;

    // Iterate over the board to find all pieces of the specified color
    for (int x = 0; x < 8; ++x) {  // Assuming an 8x8 board
        for (int y = 0; y < 8; ++y) {
            shared_ptr<Piece> piece = board.getPiece(x, y);
            
            // Skip if there is no piece or the piece is not owned by the AI
            if (!piece || piece->getColor() != color) {
                continue;
            }

            // Get the legal moves for this piece
            vector<Move> pieceMoves = piece->getLegalMoves(x, y);

            // Check each move for legality (whether it results in check)
            for (const Move& move : pieceMoves) {
                // Make the move temporarily and check if it causes the player's king to be in check
                if (!isKingInCheck(color)) {
                    legalMoves.push_back(move);
                }
            }
        }
    }

    return legalMoves;
}

// bool AI::isKingInCheck(int color) {
//     // Check if the king of the given color is in check after a move
//     // This will require a method to simulate the move and check the king's safety
//     // You can implement this by temporarily making the move and checking the board state

//     // Example: Check if the king's position is under attack
//     // (You'll need to implement this logic in your Board or Piece class)
//     // For now, we assume you have a `isKingInCheck` method in the Board class
//     return board.isKingInCheck(color);
// }

// Function to generate a random move for the AI
Move AI::generateMove(Board& board) {
    possibleMoves.clear();
    // Get the list of legal moves for the AI
    possibleMoves = board.getLegalMovesForPlayer(playerColor);

    // If there are no legal moves, return a default move (like passing or no move)
    if (possibleMoves.empty()) {
        cout << "No possible moves for AI!" << std::endl;
        return Move();  // Returning an empty move (assuming Move has a default constructor)
    }

    // Select a random move from the list of possible moves
    int randomIndex = std::rand() % possibleMoves.size();
    return possibleMoves[randomIndex];
}

// Function to check if a move is valid
bool AI::isMoveValid(const Move& move) {
    // Ensure the piece exists at the starting position
    shared_ptr<Piece> piece = board.getPiece(move.startX, move.startY);
    if (!piece) {
        cout << "No piece at starting position (" << move.startX << ", " << move.startY << ")!" << endl;
        return false;
    }

    // Check if the move is valid for the piece
    if (!piece->isValidMove(move.startX, move.startY, move.endX, move.endY)) {
        cout << "Invalid move for " << piece->getSymbol() << " from (" 
             << move.startX << ", " << move.startY << ") to (" 
             << move.endX << ", " << move.endY << ")!" << endl;
        return false;
    }

    return true;
}

//     // Optionally check if the move is valid using Move's own validation method
//     // (Ensure that Move class has the `isValid` method)
//     if (!move.isValid()) {
//         cout << "Move is not valid according to general rules!" << endl;
//         return false;
//     }

//     return true;
// }

// Optional: Evaluate the board for more complex strategies (could be used for Minimax)
// int AI::evaluateBoard(const Board& board) {
//     // A simple evaluation function could return a score based on the number of pieces on the board
//     // You can later expand this to consider board positions, piece types, etc.
//     return board.getPieceCount(playerColor) - board.getPieceCount(3 - playerColor);  // Assuming '3' is the opponent color
// }
