// AI.h
#ifndef AI_H
#define AI_H

#include <vector>
#include <memory>
#include "Board.h" // Assuming Board class is responsible for game state

using namespace std;
class AI
{

private:
    Board &board; // Reference to the board object

public:
    AI(Board& b, int playerColor);


    bool isMoveValid(const Move& move);
    Move generateMove(Board &board); // Function to generate the next move
vector<Move> getLegalMovesForPlayer(int color);

    // Optional: Functions for implementing more advanced AI logic later
    Move minimax(Board &board);
    Move monteCarlo(Board &board);

private:
    int playerColor; // The AI's color (Black or White)

    // Helper functions to support move generation
    
    int evaluateBoard(const Board &board); // Can be used for more advanced AI strategies

    // Data structures for AI (could be trees, stacks, queues, etc.)
    vector<Move> possibleMoves;
    // Could add a tree structure or a stack of previously explored moves
};

#endif // AI_H
