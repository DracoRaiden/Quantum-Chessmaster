#include "AI.h"
#include "Board.h"
#include "Piece.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
// #include <queue>


MoveList::MoveList() : head(nullptr), tail(nullptr) {}

void MoveList::addMove(const pair<pair<int, int>, pair<int, int>> &move)
{
    MoveNode *newNode = new MoveNode(move);
    if (head == nullptr)
    {
        head = tail = newNode;
    }
    else
    {
        tail->next = newNode;
        tail = newNode;
    }
}


vector<pair<pair<int, int>, pair<int, int>>> MoveList ::getAllMoves()
{
    vector<pair<pair<int, int>, pair<int, int>>> moves;
    MoveNode *current = head;
    while (current != nullptr)
    {
        moves.push_back(current->move); // Add each move to the vector
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
MoveNode::MoveNode(const pair<pair<int, int>, pair<int, int>> move) : move(move), next(nullptr) {}

CircularQueue::CircularQueue(int size) : size(size) {}

bool CircularQueue::isEmpty() const
{
    return q.empty(); // Return true if the queue is empty
}

bool CircularQueue::isMoveRecent(pair<pair<int, int>, pair<int, int>> move)
{
    // Convert both parts of the move pair into strings
    string moveStr = to_string(move.first.first) + "," + to_string(move.first.second) + " -> " +
                     to_string(move.second.first) + "," + to_string(move.second.second);
    
    // Check if the move exists in the seenMoves set
    return seenMoves.find(moveStr) != seenMoves.end();
}

void CircularQueue::addMove(pair<std::pair<int, int>, std::pair<int, int>> move)
{
    if (q.size() == size)
    {
        // Remove the oldest move from the queue and `seenMoves` if the queue is full
        string oldMoveStr = to_string(q.front().first.first) + "," + std::to_string(q.front().first.second) +
                            " -> " + std::to_string(q.front().second.first) + "," + std::to_string(q.front().second.second);
        seenMoves.erase(oldMoveStr);
        q.pop();
    }

    // Add the new move to the queue
    q.push(move);

    // Insert the string representation of the new move into seenMoves
    string moveStr = to_string(move.first.first) + "," + to_string(move.first.second) +
                     " -> " + to_string(move.second.first) + "," + to_string(move.second.second);
    seenMoves.insert(moveStr);
}


AI::AI(int moveHistorySize) : moveHistory(moveHistorySize) {}

void MoveList ::clear_moves()
{
    while (head != nullptr)
    {
        MoveNode *temp = head;
        head = head->next;
        delete temp;
    }
    tail = nullptr; // Reset tail as well
}
pair<pair<int, int>, pair<int, int>> AI::getRandomMove(MoveList &moveList)
{
    vector<pair<pair<int, int>, pair<int, int>>> moves = moveList.getAllMoves();

    if (moves.empty())
    {
        throw std::runtime_error("No moves available in MoveList."); // Handle empty list case
    }

    // Randomly pick a move
    static bool seedInitialized = false; // Ensure seeding happens only once
    if (!seedInitialized)
    {
        srand(static_cast<unsigned>(time(0)));
        seedInitialized = true;
    }

    int randomIndex = rand() % moves.size(); // Get a random index
    return moves[randomIndex];               // Return the randomly selected move
}

void AI::exploreMovesBFS(pair<int, int> startMove, const Board &board)
{
    Queue<pair<int, int>> q;
    q.push(startMove);
    CircularQueue recentMoves(10); // Limiting recent moves to last 10 for efficiency

    while (!q.empty())
    {
        auto currentMove = q.front();
        q.pop();
        cout << "Exploring move: (" << currentMove.first << ", " << currentMove.second << ")\n";
        shared_ptr<Piece> currentPiece = board.getPiece(currentMove.first, currentMove.second); // Get pointer to the piece

        // Check if the current position has a piece
        if (!currentPiece)
        {
            cout << "No piece at the starting position (" << currentMove.first << ", " << currentMove.second << "). Skipping...\n";
            continue; // Skip this iteration if there is no piece at the starting position
        }

        // Get all legal moves from currentMove
        vector<pair<int, int>> legalMoveList = currentPiece->getLegalMoves(currentMove.first, currentMove.second, board);

        // Add legal moves to queue if not recently made
        for (const auto &move : legalMoveList)
        {
            // Create a pair of pairs for the move
            pair<pair<int, int>, pair<int, int>> movePair = {{currentMove.first, currentMove.second}, move};

            // Avoid repeating moves recently made
            if (!recentMoves.isMoveRecent(movePair)) 
            {
                possibleMoves.addMove(movePair); // Add the move pair
                q.push(move); // Add the move to the queue
                recentMoves.addMove(movePair); // Use movePair here instead of move
            }
        }
    }
}


void AI::sortMovesByPriority(vector<pair<pair<int, int>, pair<int, int>>> &moves, const Board &board)
{
    // Example: prioritize capturing moves or moves with higher impact
    std::sort(moves.begin(), moves.end(), [&](const pair<pair<int, int>, pair<int, int>> &a, const pair<pair<int, int>, pair<int, int>> &b)
              {
                  // Placeholder logic: prioritize based on target position or capturing
                  shared_ptr<Piece> pieceA = board.getPiece(a.second.first, a.second.second);
                  shared_ptr<Piece> pieceB = board.getPiece(b.second.first, b.second.second);

                  int priorityA = (pieceA != nullptr) ? 1 : 0; // Higher priority for capturing
                  int priorityB = (pieceB != nullptr) ? 1 : 0;

                  return priorityA > priorityB; // Sort descending by priority
              });
}

bool MoveList::isEmpty()
{
    return head == nullptr; // Return true if the head is null, indicating the list is empty
}

void AI::generatePossibleMoves(const Board &board)
{
    possibleMoves.clear_moves(); // Ensure it's cleared before adding new moves.

    for (int x = 0; x < 8; ++x)
    {
        for (int y = 0; y < 8; ++y)
        {
            shared_ptr<Piece> piece = board.getPiece(x, y);
            if (piece != nullptr && piece->getColor() == false)
            { // Assuming AI plays black
                // Generate legal moves for this piece
                vector<pair<int, int>> legalMoves = piece->getLegalMoves(x, y, board);
                for (const auto &move : legalMoves)
                {
                    // Validate move to ensure it doesn’t leave the board or violate rules
                    if (piece->isValidMove(x, y, move.first, move.second))
                    {
                        possibleMoves.addMove({{x, y}, move});
                    }
                }
            }
        }
    }

    if (possibleMoves.isEmpty())
    {
        cout << "No possible moves generated for AI." << endl;
    }
}

// // Function to explore possible future moves using the game tree
//     void AI :: buildGameTree(const Board& board, int depth, GameTreeNode* currentNode) {
//         if (depth == 0) return; // Base case: no more moves to explore
        
//         generatePossibleMoves(board);  // Populate the list of possible moves

//         // For every possible move, create a new game state and add it as a child
//         for (const auto& move : possibleMoves.getAllMoves()) {
//             Board newBoard = board;  // Copy the current board
//             // Apply the move to the new board
//             // (Note: You would need to actually apply the move here)
            
//             // Create a new node for this state
//             GameTreeNode* newNode = new GameTreeNode(newBoard);
//             currentNode->addChild(newNode);
            
//             // Recurse to explore deeper moves
//             buildGameTree(newBoard, depth - 1, newNode);
//         }
//     }


pair<pair<int, int>, pair<int, int>> AI::selectMove(const Board &board)
{
    // Step 1: Generate possible moves
    generatePossibleMoves(board);

    // Step 2: Retrieve all possible moves
    vector<pair<pair<int, int>, pair<int, int>>> moves = possibleMoves.getAllMoves();
    if (moves.empty())
    {
        // cout << "No moves available for AI. Returning default." << endl;
        return {{-1, -1}, {-1, -1}}; // Handle case where AI has no valid moves
    }

    // cout << "Possible moves:\n";
    // for (const auto &move : moves)
    // {
    //     cout << "Move: " << move.first.first << "," << move.first.second
    //          << " -> " << move.second.first << "," << move.second.second << endl;
    // }

    // Step 3: Sort moves by priority (optional optimization)
    sortMovesByPriority(moves, board);
    // cout << "Moves sorted by priority (if applicable):\n";
    // for (const auto &move : moves)
    // {
    //     cout << "Sorted Move: " << move.first.first << "," << move.first.second
    //          << " -> " << move.second.first << "," << move.second.second << endl;
    // }

    // Step 4: If this is the first move, pick the highest-priority move
    if (moveHistory.isEmpty())
    {
        auto firstMove = moves.front();
        moveHistory.addMove(firstMove); // Assuming `addMove` expects pair<pair<int, int>, pair<int, int>>
        // cout << "First move selected: " << firstMove.first.first << "," << firstMove.first.second
        //      << " -> " << firstMove.second.first << "," << firstMove.second.second << endl;
        return firstMove;
    }

    // Step 5: Check for a move not recently made
    // cout << "Checking for a move not recently made...\n";
    for (const auto &move : moves)
    {
        // cout << "Checking move: " << move.first.first << "," << move.first.second
        //      << " -> " << move.second.first << "," << move.second.second << endl;
        
        if (!moveHistory.isMoveRecent(move)) // Assuming `isMoveRecent` expects pair<pair<int, int>, pair<int, int>>
        {
            moveHistory.addMove(move); // Assuming `addMove` expects pair<pair<int, int>, pair<int, int>>
            // cout << "Move selected: " << move.first.first << "," << move.first.second
            //      << " -> " << move.second.first << "," << move.second.second << endl;
            return move;
        }
        else
        {
            // cout << "Move recently made, skipping: " << move.first.first << "," << move.first.second
            //      << " -> " << move.second.first << "," << move.second.second << endl;
        }
    }

    // Step 6: No non-recent moves found, pick a random valid move
    // cout << "No non-recent moves found. Picking a random valid move...\n";
    auto randomMove = getRandomMove(possibleMoves);
    moveHistory.addMove(randomMove); // Assuming `addMove` expects pair<pair<int, int>, pair<int, int>>
    // cout << "Random move selected: " << randomMove.first.first << "," << randomMove.first.second
    //      << " -> " << randomMove.second.first << "," << randomMove.second.second << endl;
    return randomMove;
}

bool AI::isMoveValid(const Move &move, Board &board)
{
    // Initialize Checkmate with the board's current state
    Checkmate checkmate(board.getBoard()); // Pass board reference to Checkmate

    cout << "Checking move validity: " << move.startX << "," << move.startY << " -> " << move.endX << "," << move.endY << endl;

    // Check if the move is within board bounds
    if (move.startX < 0 || move.startX >= 8 || move.startY < 0 || move.startY >= 8 ||
        move.endX < 0 || move.endX >= 8 || move.endY < 0 || move.endY >= 8)
    {
        cout << "Move is out of bounds!" << endl;
        return false;
    }

    // Get the piece at the starting position
    shared_ptr<Piece> piece = board.getPiece(move.startX, move.startY);
    if (piece == nullptr)
    {
        cout << "No piece at the start position!" << endl;
        return false; // No piece at the starting position
    }

    // Ensure that the AI's piece is being moved
    if (piece->getColor() != false)
    {
        cout << "Not an AI piece!" << endl;
        return false; // Not an AI piece
    }

    // Check if the move is valid for the piece using the movePiece function (without actually making the move)
    if (!board.movePiece(move.startX, move.startY, move.endX, move.endY))
    {
        cout << "Invalid move based on the board logic!" << endl;
        return false; // Invalid move based on the logic defined in movePiece
    }

    return true; // If no issues, the move is valid
}
