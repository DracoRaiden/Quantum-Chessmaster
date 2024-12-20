#include "AI.h"
#include "Board.h"
#include "Piece.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <string>

MoveList::MoveList() : head(nullptr) {}

void MoveList::addMove(pair<int, int> move)
{
    MoveNode *newNode = new MoveNode(move);
    newNode->next = head; // Insert at the head of the list
    head = newNode;
}

vector<pair<int, int>> MoveList::getAllMoves()
{
    vector<pair<int, int>> moves;
    MoveNode *current = head;
    while (current != nullptr)
    {
        moves.push_back(current->move); // Add move to the vector
        current = current->next;        // Move to the next node
    }
    return moves;
}

MoveList::~MoveList()
{
    MoveNode *current = head;
    while (current != nullptr)
    {
        MoveNode *nextNode = current->next;
        delete current; // Clean up the current node
        current = nextNode;
    }
}

MoveNode::MoveNode(std::pair<int, int> move) : move(move), next(nullptr) {}

CircularQueue::CircularQueue(int size) : size(size) {}

bool CircularQueue::isEmpty() const
{
    return q.empty(); // Return true if the queue is empty
}

bool CircularQueue::isMoveRecent(std::pair<int, int> move)
{
    string moveStr = to_string(move.first) + "," + to_string(move.second);
    return seenMoves.find(moveStr) != seenMoves.end();
}

void CircularQueue::addMove(std::pair<int, int> move)
{
    if (q.size() == size)
    {
        std::string oldMoveStr = std::to_string(q.front().first) + "," + std::to_string(q.front().second);
        seenMoves.erase(oldMoveStr);
        q.pop();
    }
    q.push(move);
    seenMoves.insert(std::to_string(move.first) + "," + std::to_string(move.second));
}

AI::AI(int moveHistorySize) : moveHistory(moveHistorySize) {}

void AI::generatePossibleMoves(const Board &board)
{
    // Generate all possible legal moves for the current AI piece
    // Example for Pawn, Queen, etc. (depends on piece type)
    // Add all possible moves to `possibleMoves` linked list
}

pair<int, int> AI::getRandomMove(MoveList &moveList)
{
    vector<pair<int, int>> moves = moveList.getAllMoves();

    // Hash map to shuffle the moves
    std::unordered_map<int, std::pair<int, int>> hashedMoves;
    for (const auto &move : moves)
    {
        int hashValue = std::hash<std::string>{}(std::to_string(move.first) + "," + std::to_string(move.second));
        hashedMoves[hashValue] = move;
    }

    // Randomly pick a move based on hash values
    srand(time(0));
    auto it = hashedMoves.begin();
    std::advance(it, rand() % hashedMoves.size());
    return it->second;
}

void AI::exploreMovesBFS(pair<int, int> startMove, const Board &board)
{
    queue<pair<int, int>> q;
    q.push(startMove);

    while (!q.empty())
    {
        auto currentMove = q.front();
        q.pop();
        cout << "Exploring move: (" << currentMove.first << ", " << currentMove.second << ")\n";
        shared_ptr<Piece> currentPiece = board.getPiece(currentMove.first, currentMove.second); // Get pointer to the piece

        // Get all legal moves from currentMove and add to queue
        MoveList legalMoves;
        // (Assuming you have a method getLegalMoves that returns a list of moves)
        vector<pair<int, int>> legalMoveList = currentPiece->getLegalMoves(currentMove.first, currentMove.second);
        for (const auto &move : legalMoveList)
        {
            legalMoves.addMove(move);
            q.push(move);
        }
    }
}

void AI::sortMovesByPriority(vector<pair<int, int>> &moves, const Board &board)
{
    // Here, we can define custom logic to assign priority to moves
    // For example, moves that capture an opponent piece can have a higher priority
    std::sort(moves.begin(), moves.end(), [](const std::pair<int, int> &a, const std::pair<int, int> &b)
              {
                  // Example: prioritize capturing moves (you can adjust this logic)
                  return a.first < b.first; // Placeholder sorting logic
              });
}

pair<int, int> AI::selectMove(const Board& board) {
    generatePossibleMoves(board);

    // Sort moves by priority (optional)
    vector<std::pair<int, int>> moves = possibleMoves.getAllMoves();
    sortMovesByPriority(moves, board);

    // If there are no moves in the history (first move of the game)
    if (moveHistory.isEmpty()) {
        // Just return the first move (no history to check)
        std::pair<int, int> firstMove = moves.front();
        moveHistory.addMove(firstMove);  // Store the move in history
        return firstMove;
    }

    // Check if any move is too recent (in move history)
    for (auto& move : moves) {
        if (!moveHistory.isMoveRecent(move)) {
            // Add the move to history and return it
            moveHistory.addMove(move);
            return move;
        }
    }

    // If no valid move found, pick a random move
    return getRandomMove(possibleMoves);
}
