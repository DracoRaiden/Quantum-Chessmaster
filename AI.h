#ifndef AI_H
#define AI_H

#include <vector>
#include <unordered_map>
// #include <queue>
#include <unordered_set>
#include <utility>
#include "Board.h" // Include the Board class for move generation
#include "Piece.h" // Include the Piece
#include "Stack.h"
#include "Queue.h"

using namespace std;

class Board;
class Piece;
struct Move;

// Circular Queue class to store recent moves
class CircularQueue
{
public:
    int size;
    Queue<pair<pair<int, int>, pair<int, int>>> q;  // Change q to hold the correct type
    unordered_set<string> seenMoves;

    CircularQueue(int size);
    bool isMoveRecent(pair<pair<int, int>, pair<int, int>> move);
    void addMove(pair<pair<int, int>, pair<int, int>> move);
    bool isEmpty() const;
};

class MoveNode
{
public:
    pair<pair<int, int>, pair<int, int>> move; // Start and end positions
    MoveNode *next;                            // Pointer to the next MoveNode

    MoveNode(const pair<pair<int, int>, pair<int, int>> move);
};

class MoveList
{
public:
    MoveList();  // Constructor to initialize the head of the list
    ~MoveList();

    void addMove(const pair<pair<int, int>, pair<int, int>> &move);                                    // Add a move to the list
    vector<pair<pair<int, int>, pair<int, int>>> getAllMoves(); // Get all moves in a vector
    void clear_moves();
    bool isEmpty();

private:
    MoveNode *head; // Head pointer to the linked list of moves
    MoveNode *tail; // Points to the last node
};


// class GameTreeNode {
// public:
//     Board boardState; // The game board after a move
//     int score;        // A simple score for the state (this could be improved with evaluations)
//     GameTreeNode* parent; // Pointer to the parent node in the tree
//     vector<GameTreeNode*> children; // Children representing possible future moves
//     pair<pair<int, int>, pair<int, int>> move; // The move that led to this state

//     GameTreeNode(Board &board, int score = 0, GameTreeNode* parent = nullptr)
//         : boardState(board), score(score), parent(parent), move({{-1, -1}, {-1, -1}}) {}
    
//     // Constructor for child nodes (takes the move as well)
//     GameTreeNode(const Board &board, int score = 0, const pair<pair<int, int>, pair<int, int>> &move)
//         : boardState(board),score(score) , move(move) {}

//     // Add a child node (representing a move in the future)
//     void addChild(GameTreeNode* child) {
//         children.push_back(child);
//     }

//     // Recursively delete the tree
//     ~GameTreeNode() {
//         for (auto child : children) {
//             delete child;
//         }
//     }
// };

// AI Class to manage the AI's decision-making
class AI
{
private:
    CircularQueue moveHistory;
    MoveList possibleMoves;
    // GameTreeNode* gameTreeRoot; // The root of the game tree

public:
   AI(int moveHistorySize);

    void generatePossibleMoves(const Board &board);
    pair<pair<int, int>, pair<int, int>> selectMove(const Board &board);
    void sortMovesByPriority(vector<pair<pair<int, int>, pair<int, int>>> &moves, const Board &board);
    pair<pair<int, int>, pair<int, int>> getRandomMove(MoveList &moveList);
    void exploreMovesBFS(pair<int, int> startMove, const Board &board);
    bool isMoveValid(const Move &move, Board &board);
    // void buildGameTree(const Board& board, int depth, GameTreeNode* currentNode);
};


#endif // AI_H