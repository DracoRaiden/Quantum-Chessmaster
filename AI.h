#ifndef AI_H
#define AI_H

#include <vector>
#include <unordered_map>
#include <queue>
#include <unordered_set>
#include <utility>
#include "Board.h"  // Include the Board class for move generation
#include "Piece.h"  // Include the Piece

using namespace std;


// Definition for a linked list node to store moves
struct MoveNode {
    std::pair<int, int> move;  // The move (x, y) pair
    MoveNode* next;
    MoveNode(std::pair<int, int> move) : move(move), next(nullptr) {}
};

// Linked list class to store moves
class MoveList {
public:
    MoveNode* head;

    MoveList();
    void addMove(std::pair<int, int> move);
    std::vector<std::pair<int, int>> getAllMoves();
    ~MoveList();
};

// Circular Queue class to store recent moves
class CircularQueue {
public:
    int size;
    std::queue<std::pair<int, int>> q;
    std::unordered_set<std::string> seenMoves;

    CircularQueue(int size);
    bool isMoveRecent(std::pair<int, int> move);
    void addMove(std::pair<int, int> move);
    bool CircularQueue::isEmpty() const;
    
};

// AI Class to manage the AI's decision-making
class AI {
private:
    CircularQueue moveHistory;
    MoveList possibleMoves;

public:
    AI(int moveHistorySize);

    void generatePossibleMoves(const Board& board);
    pair<int, int> selectMove(const Board& board);
    void sortMovesByPriority(std::vector<std::pair<int, int>>& moves, const Board& board);
    pair<int, int> getRandomMove(MoveList& moveList);
    void exploreMovesBFS(std::pair<int, int> startMove, const Board& board);
    
};

class MoveNode {
public:
    std::pair<int, int> move;  // A move, which is a pair of coordinates (x, y)
    MoveNode* next;  // Pointer to the next MoveNode

    MoveNode(std::pair<int, int> move);
};

class MoveList {
public:
    MoveList();  // Constructor to initialize the head of the list
    ~MoveList();  // Destructor to clean up memory
    
    void addMove(pair<int, int> move);  // Add a move to the list
    vector<pair<int, int>> getAllMoves();  // Get all moves in a vector

private:
    MoveNode* head;  // Head pointer to the linked list of moves
};


#endif // AI_H
