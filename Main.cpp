#include <iostream>
#include "Board.h"
#include "AI.h"
#include "Piece.h"
#include "Checkmate.h"
using namespace std;

int main()
{
    Board chessBoard;
    chessBoard.setupBoard();
    int moveHistorySize = 100;
    AI aiPlayer(moveHistorySize); // moveHistorySize is the size of the circular queue for move history
    // // Initialize Checkmate with the board's current state
    // Checkmate checkmate(chessBoard.getBoard()); // Pass board reference to Checkmate

    string input, target, command;
    int gameMode = 0;           // 1 for Player vs Player, 2 for Player vs AI
    int currentPlayer = 1;      // 1 for Player 1 (White), 2 for Player 2 (Black)
    bool firstMoveMade = false; // Track if the first move has been made
    bool redoMoveMade;
    cout << "Welcome to Chess!" << endl;
    cout << "Select Game Mode:\n1. Player vs Player\n2. Player vs AI\nEnter 1 or 2: ";
    cin >> gameMode;

    if (gameMode != 1 && gameMode != 2)
    {
        cout << "Invalid selection. Exiting game." << endl;
        return 0;
    }

    while (true)
    {
        if (command != "undo" && command != "quit")
        {
            // Removed system("cls"); to prevent clearing the screen
        }

        cout << "Welcome to Chess (" << (gameMode == 1 ? "Player vs. Player" : "Player vs. AI") << ")!" << endl;
        chessBoard.printBoard();

        // Reset attack flags at the start of each turn
        chessBoard.resetAttackFlags();
        // Player's turn
        if (gameMode == 2 && currentPlayer == 2) // AI's turn if mode is Player vs AI and currentPlayer is 2
        {
            // Call AI's move
            pair<pair<int, int>, pair<int, int>> aiMove = aiPlayer.selectMove(chessBoard);
            auto [start, end] = aiMove;
            cout << "AI moves: " << start.first << "," << start.second << " -> " << end.first << "," << end.second << endl;
            chessBoard.movePiece(start.first, start.second, end.first, end.second);

            currentPlayer = 1; // Switch back to Player 1 after AI's move
            continue;          // Skip player input when AI plays
        }

        cout << "Player " << currentPlayer << "'s turn. Enter your move (e.g., e2 e4), 'undo' to undo last move, 'redo' to redo undone move, or 'quit' to exit: ";
        cin >> command;
        if (command == "quit")
        {
            cout << "Game ended. Thanks for playing!" << endl;
            break;
        }

        if (command == "undo")
        {
            // Check if there are moves to undo (history size > 1).
            if (chessBoard.getHistorySize() < 1)
            {
                cout << "No moves to undo!" << endl;
            }
            else
            {
                chessBoard.undoMove();                        // Undo the last move
                currentPlayer = (currentPlayer == 1) ? 2 : 1; // Switch back the player after undo
                cout << "Last move has been undone!" << endl;
            }
            continue;
        }
        else if (command == "redo")
        {
            // Check if there are moves to redo (redoHistory not empty).
            if (chessBoard.isRedoEmpty())
            {
                cout << "No moves to redo!" << endl;
            }
            else
            {
                chessBoard.redoMove();                        // Redo the last undone move
                currentPlayer = (currentPlayer == 1) ? 2 : 1; // Switch turns forward
                cout << "Last undone move has been redone!" << endl;
            }
            continue;
        }

        else if (command == "captured")
        {
            cout << "Captured pieces: " << endl;
            chessBoard.printCapturedPieces(); // Print captured pieces
            continue;
        }

        cin >> target; // Read the target position

        try
        {
            auto [startY, startX] = convertToIndex(command);
            auto [endY, endX] = convertToIndex(target);

            if (startX == endX && startY == endY)
            {
                cout << "Invalid move. You must move the piece to a new position! Try again." << endl;
                continue;
            }

            shared_ptr<Piece> piece = chessBoard.getPiece(startX, startY);

            if (piece == nullptr)
            {
                cout << "No piece at the starting position!" << endl;
                continue;
            }

            if ((currentPlayer == 1 && piece->isBlack()) ||
                (currentPlayer == 2 && !piece->isBlack()))
            {
                cout << "It's Player " << currentPlayer << "'s turn, but you can't move the opponent's piece!" << endl;
                continue;
            }

            if (chessBoard.movePiece(startX, startY, endX, endY))
            {
                firstMoveMade = true;
                currentPlayer = (currentPlayer == 1) ? 2 : 1; // Switch turns after a successful move
            }
            else
            {
                cout << "Move failed. Invalid move or rules violation. Try again." << endl;
            }
        }
        catch (const invalid_argument &e)
        {
            cout << "Invalid input format. " << e.what() << endl;
        }
        catch (const out_of_range &e)
        {
            cout << "Move out of bounds. " << e.what() << endl;
        }

        // // Check if the king is in check
        // if (checkmate.isInCheck())
        // {
        //     std::cout << "The king is in check!" << std::endl;
        // }
        // else
        // {
        //     std::cout << "The king is safe!" << std::endl;
        // }

        // // Check if the player is in checkmate
        // if (checkmate.isCheckmate())
        // {
        //     std::cout << "Checkmate! The game is over!" << std::endl;
        // }
        // else
        // {
        //     std::cout << "No checkmate yet!" << std::endl;
        // }
        //     // Check for game-ending conditions (Optional, can be added)
        // if (chessBoard.isGameOver())
        // {
        //     cout << "Game over! Player " << (currentPlayer == 1 ? 2 : 1) << " wins!" << endl;
        //     break;
        // }
    }

    return 0;
}
