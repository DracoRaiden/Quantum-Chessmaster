// AI.h
#ifndef AI_H
#define AI_H

#include <vector>
#include <memory>
#include "Board.h"  // Assuming Board class is responsible for game state

class AI {
public:
    AI(int playerColor);  // Constructor, takes color (black or white)
    
    Move generateMove(Board& board);  // Function to generate the next move

    // Optional: Functions for implementing more advanced AI logic later
    Move minimax(Board& board);
    Move monteCarlo(Board& board);
    
private:
    int playerColor;  // The AI's color (Black or White)
    
    // Helper functions to support move generation
    bool isMoveValid(const Move& move);
    int evaluateBoard(const Board& board);  // Can be used for more advanced AI strategies
    
    // Data structures for AI (could be trees, stacks, queues, etc.)
    std::vector<Move> possibleMoves;
    // Could add a tree structure or a stack of previously explored moves
    
};

#endif // AI_H
