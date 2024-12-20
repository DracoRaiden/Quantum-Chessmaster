#include "Board.h"
#include <memory>
#include <list>
#include <queue>
#include <stack>
#include <unordered_map>
#include <cstdlib>
#include <algorithm>
#include "Piece.h"

// ANSI color codes
const string RESET = "\033[0m";
const string WHITE_BG = "\033[47m";
const string BLACK_BG = "\033[40m";
const string WHITE_TEXT = "\033[97m";
const string BLACK_TEXT = "\033[30m";

LastMove lastMove; // Definition (with initialization)

// Board Implementation
Board::Board()
{
    board.resize(8, vector<shared_ptr<Piece>>(8, nullptr)); // Resize board to 8x8
    setupBoard();
    lastMove = {0, 0, 0, 0, false}; // Default initialization
    // After the setupBoard() call in main(), add:
    // for (int i = 0; i < 8; ++i) {
    //     for (int j = 0; j < 8; ++j) {
    //         if (board[i][j] != nullptr) {
    //             cout << "Piece at (" << i << ", " << j << "): " << typeid(*board[i][j]).name() << endl;
    //         }
    //     }
    // }
}

void Board::setupBoard()
{
    // Initialize an empty 8x8 board
    board.resize(8, vector<shared_ptr<Piece>>(8, nullptr));

    // Set up Pawns
    for (int i = 0; i < 8; ++i)
    {
        board[1][i] = make_shared<Pawn>(false); // Black pawns
        board[6][i] = make_shared<Pawn>(true);  // White pawns
    }

    // Set up Rooks
    board[0][0] = make_shared<Rook>(false);
    board[0][7] = make_shared<Rook>(false);
    board[7][0] = make_shared<Rook>(true);
    board[7][7] = make_shared<Rook>(true);

    // Set up Knights
    board[0][1] = make_shared<Knight>(false);
    board[0][6] = make_shared<Knight>(false);
    board[7][1] = make_shared<Knight>(true);
    board[7][6] = make_shared<Knight>(true);

    // Set up Bishops
    board[0][2] = make_shared<Bishop>(false);
    board[0][5] = make_shared<Bishop>(false);
    board[7][2] = make_shared<Bishop>(true);
    board[7][5] = make_shared<Bishop>(true);

    // Set up Queens
    board[0][3] = make_shared<Queen>(false);
    board[7][3] = make_shared<Queen>(true);

    // Set up Kings
    board[0][4] = make_shared<King>(false);
    board[7][4] = make_shared<King>(true);
}

void Board::printBoard() const
{
    cout << "  a b c d e f g h" << endl;
    for (int i = 0; i < 8; ++i)
    {
        cout << 8 - i << " "; // Row numbers
        for (int j = 0; j < 8; ++j)
        {
            // Alternate square colors
            bool isWhiteSquare = (i + j) % 2 == 0;
            string bgColor = isWhiteSquare ? WHITE_BG : BLACK_BG;
            string textColor = isWhiteSquare ? BLACK_TEXT : WHITE_TEXT;

            if (board[i][j] != nullptr)
            {
                // Display piece with symbol (alphabetic representation) and color
                cout << bgColor << textColor << board[i][j]->getSymbol() << " " << RESET;
            }
            else
            {
                // Display empty square with color
                cout << bgColor << "  " << RESET;
            }
        }
        cout << 8 - i << endl; // Row numbers again
    }
    cout << "  a b c d e f g h" << endl;
}

bool Board::isSquareOccupied(int x, int y) const
{
    // cout << "Checking if square (" << x << ", " << y << ") is occupied..." << endl;
    return board[x][y] != nullptr;
}

bool Board::isPathClear(int startX, int startY, int endX, int endY) const
{
    // If the move is a knight move, no need to check for path blockage
    int dx = abs(endX - startX);
    int dy = abs(endY - startY);

    // Knight moves in an L shape, so no need to check the path for knight
    if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2))
    {
        return true; // No path check for knight moves
    }

    // For other pieces, check if path is clear
    dx = (endX - startX) == 0 ? 0 : (endX - startX) / abs(endX - startX);
    dy = (endY - startY) == 0 ? 0 : (endY - startY) / abs(endY - startY);

    int currentX = startX + dx;
    int currentY = startY + dy;

    // Traverse through all squares between start and end position
    while (currentX != endX || currentY != endY)
    {
        if (isSquareOccupied(currentX, currentY))
        {
            return false; // Path is blocked
        }
        currentX += dx;
        currentY += dy;
    }

    return true; // Path is clear
}

bool Board::movePiece(int startX, int startY, int endX, int endY)
{
    // Save the current board state before making the move (for undo functionality)
    saveHistory();

    if (!isSquareOccupied(startX, startY))
    {
        cout << "No piece at the starting position!" << endl;
        return false;
    }

    // Get the piece at the starting position
    auto piece = board[startX][startY];

    // Ensure the move is within board bounds
    if (startX < 0 || startX >= 8 || startY < 0 || startY >= 8 ||
        endX < 0 || endX >= 8 || endY < 0 || endY >= 8)
    {
        cout << "Move is out of bounds!" << endl;
        return false;
    }

    // Castling Logic: If the king moves two squares horizontally, check for castling
    if ((piece->getSymbol() == 'K' || piece->getSymbol() == 'k') && abs(endY - startY) == 2)
    {
        if (canCastle(startX, startY, endX, endY))
        {
            // Determine castling side: kingside (endY > startY) or queenside (endY < startY)
            bool isKingside = endY > startY;
            int rookY = isKingside ? 7 : 0;

            // Perform castling: Move the king and the rook
            board[endX][endY] = piece;
            board[startX][startY] = nullptr;
            piece->setHasMoved(true);

            int rookNewY = isKingside ? endY - 1 : endY + 1;
            board[endX][rookNewY] = board[startX][rookY];
            board[startX][rookY] = nullptr;
            board[endX][rookNewY]->setHasMoved(true);

            cout << "Castling performed successfully!" << endl;
            return true;
        }
        else
        {
            cout << "Castling conditions not met!" << endl;
            return false;
        }
    }

    // Check if the move is valid for the piece
    if (!piece->isValidMove(startX, startY, endX, endY))
    {
        cout << "Invalid move for " << piece->getSymbol() << "!" << endl;
        return false;
    }

    // Check if the path is clear for non-knight pieces
    if (!isPathClear(startX, startY, endX, endY))
    {
        cout << "Path is blocked!" << endl;
        return false;
    }

    // Check if the destination square has a piece of the same color
    if (isSquareOccupied(endX, endY) &&
        board[endX][endY]->getColor() == piece->getColor())
    {
        cout << "Cannot capture your own piece!" << endl;
        return false;
    }

    // Handle En Passant (for Pawn)
    if (dynamic_cast<Pawn *>(piece.get()))
    {
        int direction = (piece->getColor() == 'w') ? -1 : 1; // White moves up, Black moves down

        // Check for En Passant capture
        if (abs(startY - endY) == 1 && abs(startX - endX) == 1)
        {
            if (lastMove.isTwoSquareMove && lastMove.endX == startX && endY == lastMove.startY)
            {
                board[endX][endY] = piece;
                board[startX][startY] = nullptr;
                board[lastMove.endX][lastMove.endY] = nullptr;
                cout << piece->getSymbol() << " captured en passant!" << endl;
                updateLastMove(startX, startY, endX, endY, true);
                return true;
            }
        }
    }

    // Move the piece
    board[endX][endY] = piece;
    board[startX][startY] = nullptr;

    // Handle pawn promotion
    if ((piece->getSymbol() == 'P' && endX == 0) || (piece->getSymbol() == 'p' && endX == 7))
    {
        promotePawn(endX, endY);
    }

    // Update the last move
    bool isTwoSquareMove = (abs(startX - endX) == 2); // Track if it's a two-square move
    updateLastMove(startX, startY, endX, endY, isTwoSquareMove);

    cout << piece->getSymbol() << " moved from "
         << startX << "," << startY << " to " << endX << "," << endY << "." << endl;

    return true;
}

shared_ptr<Piece> Board::getPiece(int row, int col) const
{
    if (row < 0 || row >= 8 || col < 0 || col >= 8)
    {
        cout << "Out of bounds access at row: " << row << ", col: " << col << endl;
        return nullptr;
    }
    // if (board[row][col] == nullptr) {
    //     cout << "No piece at row: " << row << ", col: " << col << endl;
    // } else {
    //     cout << "Piece found at row: " << row << ", col: " << col << endl;
    // }
    return board[row][col];
}

pair<int, int> convertToIndex(const string &position)
{
    if (position.length() != 2)
    {
        throw invalid_argument("Invalid chess notation");
    }

    char file = position[0]; // 'a' to 'h'
    char rank = position[1]; // '1' to '8'

    // Map file ('a'-'h') to column index (0-7)
    int col = file - 'a';
    if (col < 0 || col > 7)
    {
        throw out_of_range("File out of bounds");
    }

    // Map rank ('1'-'8') to row index (7-0)
    int row = 8 - (rank - '0'); // This works fine for ranks '1' to '8'

    if (row < 0 || row > 7)
    {
        throw out_of_range("Rank out of bounds");
    }

    return {col, row};
}

void Board::updateLastMove(int startX, int startY, int endX, int endY, bool isTwoSquareMove)
{
    // Update the lastMove object with the new move details
    lastMove.startX = startX;
    lastMove.startY = startY;
    lastMove.endX = endX;
    lastMove.endY = endY;
    lastMove.isTwoSquareMove = isTwoSquareMove;

    // Check if a piece is captured, if yes, store it in pieceCaptured
    if (board[endX][endY] != nullptr)
    {
        lastMove.pieceCaptured = board[endX][endY]; // Piece at the destination is captured
    }
    else
    {
        lastMove.pieceCaptured = nullptr; // No capture
    }
    // cout << "Last move: Start (" << lastMove.startX << ", " << lastMove.startY << ") -> End (" << lastMove.endX << ", " << lastMove.endY << ")" << endl
    //      << isTwoSquareMove << endl;
}

void Board::promotePawn(int x, int y)
{
    // Check if the piece at the position is a pawn
    if (!board[x][y] || (board[x][y]->getSymbol() != 'P' && board[x][y]->getSymbol() != 'p'))
    {
        cout << "No pawn at the promotion position!" << endl;
        return;
    }

    // Determine if the pawn is white or black
    bool isWhitePawn = board[x][y]->getColor(); // Using getColor() instead of checking symbol

    // Ensure the pawn is in the promotion row
    if ((isWhitePawn && x != 0) || (!isWhitePawn && x != 7))
    {
        cout << "The pawn hasn't reached the promotion rank!" << endl;
        return;
    }

    // Prompt for promotion piece choice
    cout << "Promote pawn at (" << x << ", " << y << "). Choose promotion piece (Q/R/B/N for white, q/r/b/n for black): ";
    char promotionChoice;
    cin >> promotionChoice;

    // Validate the promotion choice based on the pawn's color
    shared_ptr<Piece> newPiece;

    if (isWhitePawn)
    {
        // For white pawn (Player 1), create white pieces
        switch (toupper(promotionChoice))
        {
        case 'Q':
            newPiece = make_shared<Queen>(true);
            break; // White Queen
        case 'R':
            newPiece = make_shared<Rook>(true);
            break; // White Rook
        case 'B':
            newPiece = make_shared<Bishop>(true);
            break; // White Bishop
        case 'N':
            newPiece = make_shared<Knight>(true);
            break; // White Knight
        default:
            cout << "Invalid promotion choice for white. Defaulting to Queen." << endl;
            newPiece = make_shared<Queen>(true); // Default to White Queen
            break;
        }
    }
    else
    {
        // For black pawn (Player 2), create black pieces
        switch (tolower(promotionChoice))
        {
        case 'q':
            newPiece = make_shared<Queen>(false);
            break; // Black Queen
        case 'r':
            newPiece = make_shared<Rook>(false);
            break; // Black Rook
        case 'b':
            newPiece = make_shared<Bishop>(false);
            break; // Black Bishop
        case 'n':
            newPiece = make_shared<Knight>(false);
            break; // Black Knight
        default:
            cout << "Invalid promotion choice for black. Defaulting to Queen." << endl;
            newPiece = make_shared<Queen>(false); // Default to Black Queen
            break;
        }
    }

    // Replace the pawn with the promoted piece
    board[x][y] = newPiece;

    cout << "Pawn promoted to " << newPiece->getSymbol() << "!" << endl;
}

bool Board::canCastle(int startX, int startY, int endX, int endY) const
{
    shared_ptr<Piece> king = board[startX][startY];
    if (!king || (king->getSymbol() != 'K' && king->getSymbol() != 'k'))
    {
        cout << "Not a king!" << endl;
        return false;
    }
    if (king->getHasMoved())
    {
        cout << "King has already moved!" << endl;
        return false;
    }

    bool isKingside = (endY > startY);
    int rookY = isKingside ? 7 : 0;
    shared_ptr<Piece> rook = board[startX][rookY];
    if (!rook || rook->getSymbol() != 'R' && rook->getSymbol() != 'r' || rook->getHasMoved())
    {
        cout << "Invalid rook for castling!" << endl;
        return false;
    }

    // Ensure no pieces between the king and rook
    int step = isKingside ? 1 : -1;
    for (int y = startY + step; y != rookY; y += step)
    {
        if (board[startX][y])
        {
            cout << "Path blocked at " << startX << "," << y << "!" << endl;
            return false;
        }
    }

    // Ensure squares the king moves through are not under attack
    for (int y = startY; y != endY + step; y += step)
    {
        if (y != startY)
        {
            // Check if the square is under attack
            if (isSquareUnderAttack(startX, y, king->getColor()))
            {
                cout << "Square " << startX << "," << y << " is under attack!" << endl;
                return false;
            }
        }
    }

    return true;
}

bool Board::isSquareUnderAttack(int x, int y, bool color) const
{
    cout << "Checking square: " << x << "," << y << endl;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            shared_ptr<Piece> attacker = board[i][j];
            if (attacker && attacker->getColor() != color)
            {
                // Check if the attacker can move to (x, y)
                if (attacker->isValidMove(i, j, x, y))
                {
                    // For long-range pieces (Q, R, B), ensure they are not blocking the path
                    if (attacker->getSymbol() == 'Q' || attacker->getSymbol() == 'R' || attacker->getSymbol() == 'B')
                    {
                        if (!isPathClear(i, j, x, y))
                        {
                            continue; // Path is not clear, invalid move
                        }
                    }

                    // Skip attacking squares if they are not on the castling path
                    if (x == 7 && y == 3)
                    { // For d1 (7,3)
                        if (attacker->getSymbol() == 'q' && i == 0 && j == 3)
                        { // If the piece is the queen on d1 itself, skip
                            continue;
                        }
                    }
                    return true; // Square is under attack
                }
            }
        }
    }
    return false; // No attackers found
}

void Board::saveHistory()
{
    // Create a new vector to store the current state of the board.
    vector<vector<shared_ptr<Piece>>> currentState;

    // Iterate through each square on the board and store the current piece.
    for (int row = 0; row < 8; ++row)
    {
        vector<shared_ptr<Piece>> rowState;
        for (int col = 0; col < 8; ++col)
        {
            // Add the piece at the current position to the row.
            rowState.push_back(board[row][col]);
        }
        // Add the row to the current state.
        currentState.push_back(rowState);
    }

    // Push the current state onto the history stack.
    history.push(currentState);
}

void Board::undoMove()
{
    // Check if there is a move to undo (i.e., the history stack is not empty).
    if (!history.empty())
    {
        // Pop the top state from the history stack.
        vector<vector<shared_ptr<Piece>>> previousState = history.top();
        history.pop();

        // Restore the board to the previous state.
        for (int row = 0; row < 8; ++row)
        {
            for (int col = 0; col < 8; ++col)
            {
                // Set the board piece back to the previous state.
                board[row][col] = previousState[row][col];
            }
        }
    }
    else
    {
        // If no moves are available to undo, print an error or handle it.
        cout << "No moves to undo!" << endl;
    }
}

// vector<pair<int, int>> Board::getPossibleMoves(int startX, int startY) const
// {
//     vector<pair<int, int>> moves;

//     // Check if there is a piece at the given position
//     if (!isSquareOccupied(startX, startY))
//     {
//         return moves; // Return an empty list if no piece exists
//     }

//     // Retrieve the piece and its type
//     auto piece = getPiece(startX, startY);
//     if (!piece)
//     {
//         return moves; // Safety check
//     }

//     char pieceType = piece->getSymbol(); // Assuming getType() returns a char: 'K', 'Q', 'R', 'B', 'N', or 'P'

//     // King movement
//     if (pieceType == 'K' || pieceType == 'k')
//     {
//         for (int dx : {-1, 0, 1})
//         {
//             for (int dy : {-1, 0, 1})
//             {
//                 if (dx == 0 && dy == 0)
//                     continue;
//                 int newX = startX + dx, newY = startY + dy;
//                 if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8 &&
//                     (!isSquareOccupied(newX, newY) || getPiece(newX, newY)->getColor() != piece->getColor()))
//                 {
//                     moves.push_back({newX, newY});
//                 }
//             }
//         }
//     }

//     // Rook movement
//     else if (pieceType == 'R' || pieceType == 'r')
//     {
//         vector<pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
//         for (auto &dir : directions)
//         {
//             int newX = startX, newY = startY;
//             while (true)
//             {
//                 newX += dir.first, newY += dir.second;
//                 if (newX < 0 || newX >= 8 || newY < 0 || newY >= 8)
//                     break;
//                 if (!isSquareOccupied(newX, newY))
//                     moves.push_back({newX, newY});
//                 else
//                 {
//                     if (getPiece(newX, newY)->getColor() != piece->getColor())
//                         moves.push_back({newX, newY});
//                     break;
//                 }
//             }
//         }
//     }

//     // Bishop movement
//     else if (pieceType == 'B' || pieceType == 'b')
//     {
//         vector<pair<int, int>> directions = {{1, 1}, {-1, -1}, {1, -1}, {-1, 1}};
//         for (auto &dir : directions)
//         {
//             int newX = startX, newY = startY;
//             while (true)
//             {
//                 newX += dir.first, newY += dir.second;
//                 if (newX < 0 || newX >= 8 || newY < 0 || newY >= 8)
//                     break;
//                 if (!isSquareOccupied(newX, newY))
//                     moves.push_back({newX, newY});
//                 else
//                 {
//                     if (getPiece(newX, newY)->getColor() != piece->getColor())
//                         moves.push_back({newX, newY});
//                     break;
//                 }
//             }
//         }
//     }

//     // Queen movement
//     else if (pieceType == 'Q' || pieceType == 'q')
//     {
//         vector<pair<int, int>> directions = {
//             {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}};
//         for (auto &dir : directions)
//         {
//             int newX = startX, newY = startY;
//             while (true)
//             {
//                 newX += dir.first, newY += dir.second;
//                 if (newX < 0 || newX >= 8 || newY < 0 || newY >= 8)
//                     break;
//                 if (!isSquareOccupied(newX, newY))
//                     moves.push_back({newX, newY});
//                 else
//                 {
//                     if (getPiece(newX, newY)->getColor() != piece->getColor())
//                         moves.push_back({newX, newY});
//                     break;
//                 }
//             }
//         }
//     }

//     // Knight movement
//     else if (pieceType == 'N' || pieceType == 'n')
//     {
//         vector<pair<int, int>> directions = {
//             {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};
//         for (auto &dir : directions)
//         {
//             int newX = startX + dir.first, newY = startY + dir.second;
//             if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8 &&
//                 (!isSquareOccupied(newX, newY) || getPiece(newX, newY)->getColor() != piece->getColor()))
//             {
//                 moves.push_back({newX, newY});
//             }
//         }
//     }

//     // Pawn movement
//     else if (pieceType == 'P' || pieceType == 'p')
//     {
//         int direction = piece->getColor() ? -1 : 1;
//         int newY = startY + direction;

//         // Forward move
//         if (newY >= 0 && newY < 8 && !isSquareOccupied(startX, newY))
//         {
//             moves.push_back({startX, newY});
//         }

//         // Capture diagonals
//         for (int dx : {-1, 1})
//         {
//             int newX = startX + dx;
//             if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8 && isSquareOccupied(newX, newY))
//             {
//                 auto target = getPiece(newX, newY);
//                 if (target && target->getColor() != piece->getColor())
//                 {
//                     moves.push_back({newX, newY});
//                 }
//             }
//         }
//     }

//     return moves;
// }

// // Get all legal moves for a given player
// vector<Move> Board::getLegalMovesForPlayer(int player)
// {
//     vector<Move> legalMoves;

//     // Loop through each square on the board
//     for (int startX = 0; startX < 8; ++startX)
//     {
//         for (int startY = 0; startY < 8; ++startY)
//         {
//             // Get the piece at the current position
//             shared_ptr<Piece> piece = board[startX][startY];

//             // If there's a piece belonging to the current player, generate its possible moves
//             if (piece && piece->getColor() == player)
//             {
//                 // Get all possible moves for this piece by calling the polymorphic function
//                 vector<pair<int, int>> moves = getPossibleMoves(startX, startY);

//                 // For each possible move, check the legality
//                 for (const auto &move : moves)
//                 {
//                     int endX = move.first;
//                     int endY = move.second;

//                     // Validate move conditions
//                     // 1. The square is empty, or it contains an opponent's piece (for capturing).
//                     // 2. The path is clear (for sliding pieces like rooks, bishops, and queens).
//                     if (movePiece(startX, startY, endX, endY))
//                     {

//                         if (!isSquareOccupied(startX, startY) || !getPiece(startX, startY))
//                         {
//                             continue; // Skip invalid starting positions
//                         }

//                         legalMoves.push_back({startX, startY, endX, endY});
//                     }
//                 }
//             }
//         }
//     }

//     return legalMoves;
// }

// stack<Move> lastAIMoves;

// bool Board::isMoveRepeated(const Move &move)
// {
//     stack<Move> tempStack = lastAIMoves; // Copy the stack

//     while (!tempStack.empty())
//     {
//         const Move &lastMove = tempStack.top();
//         if (lastMove.startX == move.startX &&
//             lastMove.startY == move.startY &&
//             lastMove.endX == move.endX &&
//             lastMove.endY == move.endY)
//         {
//             return true;
//         }
//         tempStack.pop(); // Move to the next item
//     }
//     return false;
// }

// void Board::markMoveAsMade(const Move &move)
// {
//     // Temporary stack to manage a fixed size of 5 moves
//     stack<Move> tempStack;

//     // Transfer elements to the temporary stack (up to 4 moves)
//     while (!lastAIMoves.empty() && tempStack.size() < 4)
//     {
//         tempStack.push(lastAIMoves.top());
//         lastAIMoves.pop();
//     }

//     // Clear the original stack
//     while (!lastAIMoves.empty())
//     {
//         lastAIMoves.pop();
//     }

//     // Push back the moves into lastAIMoves in the correct order
//     while (!tempStack.empty())
//     {
//         lastAIMoves.push(tempStack.top());
//         tempStack.pop();
//     }

//     // Push the new move onto the stack
//     lastAIMoves.push(move);
// }

// Move Board::calculateAIMove()
// {
//     vector<Move> possibleMoves = getLegalMovesForPlayer(false); // Black (AI)

//     if (possibleMoves.empty())
//     {
//         // No moves available
//         return {-1, -1, -1, -1};
//     }

//     // Seed the random number generator
//     srand(static_cast<unsigned>(time(nullptr)));

//     // Shuffle the possible moves using simple random logic
//     for (size_t i = 0; i < possibleMoves.size(); ++i)
//     {
//         int randomIndex = rand() % possibleMoves.size();
//         swap(possibleMoves[i], possibleMoves[randomIndex]);
//     }

//     for (const Move &move : possibleMoves)
//     {
//         if (movePiece(move.startX, move.startY, move.endX, move.endY))
//         {
//             // Undo the move to maintain game state
//             undoMove();

//             // Ensure the move isn't repeated
//             if (!isMoveRepeated(move))
//             {
//                 markMoveAsMade(move);
//                 return move;
//             }
//         }
//     }

//     // No valid moves found
//     return {-1, -1, -1, -1};
// }

// Board method to get possible moves for a piece at a given position
vector<pair<int, int>> Board::getPossibleMoves(int startX, int startY) const {
    vector<pair<int, int>> moves;

    // Check if the square is occupied
    if (!isSquareOccupied(startX, startY)) {
        return moves;  // Return an empty list if no piece exists
    }

    // Retrieve the piece at the specified position
    shared_ptr<Piece> piece = getPiece(startX, startY);
    if (piece) {
        moves = piece->getLegalMoves(startX, startY, *this);
    }

    return moves;
}
