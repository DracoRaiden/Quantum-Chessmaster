#include "Board.h"
#include <memory>
#include<stack>


// ANSI color codes
const string RESET = "\033[0m";
const string WHITE_BG = "\033[47m";
const string BLACK_BG = "\033[40m";
const string WHITE_TEXT = "\033[97m";
const string BLACK_TEXT = "\033[30m";

LastMove lastMove; // Definition (with initialization)

bool Pawn::isValidMove(int startX, int startY, int endX, int endY) const
{
    int direction = isWhite ? -1 : 1; // White pawns move "up", black pawns move "down"

    // White pawn's single step forward
    if (isWhite && endX == startX + direction && endY == startY)
    {
        return true;
    }

    // Black pawn's single step forward
    if (!isWhite && endX == startX + direction && endY == startY)
    {
        return true;
    }

    // White pawn's double step forward (only from the starting row)
    if (isWhite && endX == startX + 2 * direction && endY == startY && startX == 6)
    {
        return true;
    }

    // Black pawn's double step forward (only from the starting row)
    if (!isWhite && endX == startX + 2 * direction && endY == startY && startX == 1)
    {
        return true;
    }

    // Diagonal capture (white or black)
    if (abs(startX - endX) == 1 && abs(startY - endY) == 1)
    {
        return true;
    }

    // En passant capture
    if (abs(startX - endX) == 1 && abs(startY - endY) == 1)
    {
        if (lastMove.isTwoSquareMove && lastMove.endX == startX && endY == lastMove.startY)
            return true;
    }

    return false;
}

bool Rook::isValidMove(int startX, int startY, int endX, int endY) const
{
    return (startX == endX || startY == endY); // Rook moves horizontally or vertically
}
bool Knight::isValidMove(int startX, int startY, int endX, int endY) const
{
    int dx = abs(endX - startX);
    int dy = abs(endY - startY);

    // Check if move follows "L" shape (1x2 or 2x1 move)
    if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2))
    {
        return true; // Valid knight move
    }

    return false; // Invalid move
}

bool Bishop::isValidMove(int startX, int startY, int endX, int endY) const
{
    return abs(endX - startX) == abs(endY - startY); // Diagonal moves
}

bool Queen::isValidMove(int startX, int startY, int endX, int endY) const
{
    return (startX == endX || startY == endY || abs(endX - startX) == abs(endY - startY));
}

bool King::isValidMove(int startX, int startY, int endX, int endY) const {
    // Normal King movement: one square in any direction
    if (abs(endX - startX) <= 1 && abs(endY - startY) <= 1) {
        return true;
    }

    // Castling check: two-square horizontal move
    if (startX == endX && abs(endY - startY) == 2) { // Same row, two-square move
        // Ensure the king hasn't moved before
        if (!hasMoved) {
            return true; // Castling attempt is valid at this point
        }
    }

    return false; // Move is invalid if it doesn't satisfy the above conditions
}


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


void Board::promotePawn(int x, int y) {
    // Check if the piece at the position is a pawn
    if (!board[x][y] || (board[x][y]->getSymbol() != 'P' && board[x][y]->getSymbol() != 'p')) {
        cout << "No pawn at the promotion position!" << endl;
        return;
    }

    // Determine if the pawn is white or black
    bool isWhitePawn = board[x][y]->getColor();  // Using getColor() instead of checking symbol

    // Ensure the pawn is in the promotion row
    if ((isWhitePawn && x != 0) || (!isWhitePawn && x != 7)) {
        cout << "The pawn hasn't reached the promotion rank!" << endl;
        return;
    }

    // Prompt for promotion piece choice
    cout << "Promote pawn at (" << x << ", " << y << "). Choose promotion piece (Q/R/B/N for white, q/r/b/n for black): ";
    char promotionChoice;
    cin >> promotionChoice;

    // Validate the promotion choice based on the pawn's color
    shared_ptr<Piece> newPiece;

    if (isWhitePawn) {
        // For white pawn (Player 1), create white pieces
        switch (toupper(promotionChoice)) {
            case 'Q': newPiece = make_shared<Queen>(true); break;    // White Queen
            case 'R': newPiece = make_shared<Rook>(true); break;     // White Rook
            case 'B': newPiece = make_shared<Bishop>(true); break;   // White Bishop
            case 'N': newPiece = make_shared<Knight>(true); break;   // White Knight
            default:
                cout << "Invalid promotion choice for white. Defaulting to Queen." << endl;
                newPiece = make_shared<Queen>(true);                 // Default to White Queen
                break;
        }
    } else {
        // For black pawn (Player 2), create black pieces
        switch (tolower(promotionChoice)) {
            case 'q': newPiece = make_shared<Queen>(false); break;   // Black Queen
            case 'r': newPiece = make_shared<Rook>(false); break;    // Black Rook
            case 'b': newPiece = make_shared<Bishop>(false); break;  // Black Bishop
            case 'n': newPiece = make_shared<Knight>(false); break;  // Black Knight
            default:
                cout << "Invalid promotion choice for black. Defaulting to Queen." << endl;
                newPiece = make_shared<Queen>(false);                // Default to Black Queen
                break;
        }
    }

    // Replace the pawn with the promoted piece
    board[x][y] = newPiece;

    cout << "Pawn promoted to " << newPiece->getSymbol() << "!" << endl;
}

bool Board::canCastle(int startX, int startY, int endX, int endY) const {
    shared_ptr<Piece> king = board[startX][startY];
    if (!king || (king->getSymbol() != 'K' && king->getSymbol() != 'k')) {
        cout << "Not a king!" << endl;
        return false;
    }
    if (king->getHasMoved()) {
        cout << "King has already moved!" << endl;
        return false;
    }

    bool isKingside = (endY > startY);
    int rookY = isKingside ? 7 : 0;
    shared_ptr<Piece> rook = board[startX][rookY];
    if (!rook || rook->getSymbol() != 'R' && rook->getSymbol() != 'r' || rook->getHasMoved()) {
        cout << "Invalid rook for castling!" << endl;
        return false;
    }

    // Ensure no pieces between the king and rook
    int step = isKingside ? 1 : -1;
    for (int y = startY + step; y != rookY; y += step) {
        if (board[startX][y]) {
            cout << "Path blocked at " << startX << "," << y << "!" << endl;
            return false;
        }
    }

   // Ensure squares the king moves through are not under attack
for (int y = startY; y != endY + step; y += step) {
    if (y != startY) {
        // Check if the square is under attack
        if (isSquareUnderAttack(startX, y, king->getColor())) {
            cout << "Square " << startX << "," << y << " is under attack!" << endl;
            return false;
        }
    }
}




    return true;
}



void Board::saveHistory() {
    // Create a new vector to store the current state of the board.
    vector<vector<shared_ptr<Piece>>> currentState;
    
    // Iterate through each square on the board and store the current piece.
    for (int row = 0; row < 8; ++row) {
        vector<shared_ptr<Piece>> rowState;
        for (int col = 0; col < 8; ++col) {
            // Add the piece at the current position to the row.
            rowState.push_back(board[row][col]);
        }
        // Add the row to the current state.
        currentState.push_back(rowState);
    }

    // Push the current state onto the history stack.
    history.push(currentState);
}


void Board::undoMove() {
    // Check if there is a move to undo (i.e., the history stack is not empty).
    if (!history.empty()) {
        // Pop the top state from the history stack.
        vector<vector<shared_ptr<Piece>>> previousState = history.top();
        history.pop();

        // Restore the board to the previous state.
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                // Set the board piece back to the previous state.
                board[row][col] = previousState[row][col];
            }
        }
    } else {
        // If no moves are available to undo, print an error or handle it.
        cout << "No moves to undo!" << endl;
    }
}


bool Board::isCheckmate(bool isWhite)
{
    if (!isKingInCheck(isWhite))
        return false;  // The king is not in check, so it can't be checkmate

    if (!hasLegalMoves(isWhite))  // Check if no legal moves exist
        return true;  // Checkmate condition is met

    return false;
}

bool Board::isStalemate(bool isWhite)
{
    if (isKingInCheck(isWhite))
        return false;  // If the king is in check, it is not a stalemate

    if (!hasLegalMoves(isWhite))  // Check if no legal moves exist
        return true;  // Stalemate condition is met

    return false;
}


// Function to check if the king is in check
bool Board::isKingInCheck(bool isWhite) const
{
    int kingX = -1, kingY = -1;

    // Locate the king's position
    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            shared_ptr<Piece> piece = board[y][x];
            if (piece && piece->getSymbol() == (isWhite ? 'K' : 'k'))
            {
                kingX = x;
                kingY = y;
                break;
            }
        }
    }

    if (kingX == -1 || kingY == -1)
        return false;  // King not found

    // Check if the king's position is under attack
    return isSquareUnderAttack(kingX, kingY, !isWhite);
}

// Function to check if a square is under attack
bool Board::isSquareUnderAttack(int x, int y, bool byWhite) const
{
    // Check if any opponent's piece can attack the square
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            shared_ptr<Piece> piece = board[i][j];
            if (piece && piece->getColor() == byWhite && piece->isValidMove(j, i, x, y))
            {
                return true;  // Square is under attack
            }
        }
    }
    return false;  // Square is not under attack
}

// Function to check if any move can protect the king
bool Board::canMoveToProtectKing(bool isWhite)
{
    // Now, check if the current player can make a move to protect the king.
    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            shared_ptr<Piece> piece = board[y][x];
            if (piece && piece->getColor() == isWhite)
            {
                // Try all possible moves for the piece
                for (int endY = 0; endY < 8; ++endY)
                {
                    for (int endX = 0; endX < 8; ++endX)
                    {
                        if (piece->isValidMove(x, y, endX, endY) && isPathClear(x, y, endX, endY))
                        {
                            // Simulate the move
                            auto capturedPiece = board[endY][endX];
                            board[endY][endX] = board[y][x];
                            board[y][x] = nullptr;

                            // Check if the king is still in check after the move
                            bool stillInCheck = isKingInCheck(isWhite);

                            // Undo the move
                            board[y][x] = board[endY][endX];
                            board[endY][endX] = capturedPiece;

                            // If the move blocks the check, captures the attacker or moves the king out of check
                            if (!stillInCheck)
                            {
                                return true;  // Legal move found to protect the king
                            }
                        }
                    }
                }
            }
        }
    }

    return false;  // No legal move found to protect the king
}

// Function to check if any legal moves exist for the player
bool Board::hasLegalMoves(bool isWhite)
{
    // First, check if the king is in check.
    if (isKingInCheck(isWhite)) {
        cout << "King is in check! You must move the king or block the check." << endl;

        // Check if there is any legal move that can protect the king
        if (canMoveToProtectKing(isWhite)) {
            return true;  // A move exists to protect the king
        } else {
            cout << "Player " << (isWhite ? 1 : 2) << " is in checkmate! Game over." << endl;
            return false;  // No moves exist to protect the king, checkmate
        }
    }

    // Otherwise, check for all possible moves for the current player
    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            shared_ptr<Piece> piece = board[y][x];
            if (piece && piece->getColor() == isWhite)
            {
                // Try all possible moves for the piece
                for (int endY = 0; endY < 8; ++endY)
                {
                    for (int endX = 0; endX < 8; ++endX)
                    {
                        if (piece->isValidMove(x, y, endX, endY) && isPathClear(x, y, endX, endY))
                        {
                            // Simulate the move
                            auto capturedPiece = board[endY][endX];
                            board[endY][endX] = board[y][x];
                            board[y][x] = nullptr;

                            // Check if the king is still in check after the move
                            bool stillInCheck = isKingInCheck(isWhite);

                            // Undo the move
                            board[y][x] = board[endY][endX];
                            board[endY][endX] = capturedPiece;

                            // If the move doesn't place the king in check, it's a valid move
                            if (!stillInCheck)
                            {
                                return true;  // Legal move found
                            }
                        }
                    }
                }
            }
        }
    }

    return false;  // No legal moves found
}

// Function to handle user input and validate moves
void Board::makeMove(int startX, int startY, int endX, int endY, bool isWhite)
{
    if (isKingInCheck(isWhite)) {
        cout << "King is in check! You must move the king or block the check." << endl;

        // Check if the move is valid and can protect the king
        if (canMoveToProtectKing(isWhite)) {
            // Process the move normally
            movePiece(startX, startY, endX, endY);
        } else {
            cout << "Invalid move! You must protect your king from check first." << endl;
        }
    } else {
        // Normal move processing
        movePiece(startX, startY, endX, endY);
    }
}

// Add this function to validate moves when the king is in check
  
bool Board::isValidMoveUnderCheck(int startX, int startY, int endX, int endY, bool isWhite)
{
    // Make a hypothetical move
    auto piece = board[startX][startY];
    auto target = board[endX][endY];
    board[endX][endY] = piece;
    board[startX][startY] = nullptr;

    // Check if the move resolves the check
    bool stillInCheck = isKingInCheck(isWhite);

    // Undo the hypothetical move
    board[startX][startY] = piece;
    board[endX][endY] = target;

    return !stillInCheck;
}