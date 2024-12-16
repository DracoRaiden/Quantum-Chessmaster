#include <iostream>
#include "Board.h"
using namespace std;

int main()
{
    Board chessBoard;
    chessBoard.setupBoard();

    string input, target, command;
    int gameMode = 0;      // 1 for Player vs Player, 2 for player vs AI
    int currentPlayer = 1; // 1 for Player 1 (White), 2 for Player 2 (Black)

    cout << "Welcome to Chess (Player vs. Player)!" << endl;
    cout << "Select Game Mode:\n1. Player vs Player\n2. Player vs AI\n";
    cout<<"Enter 1 or 2: ";
    cin >> gameMode;

    if (gameMode != 1 && gameMode != 2)
    {
        cout << "Invalid selection. Exiting game." << endl;
        return 0;
    }

    while (true)
    {
        // Clear the screen only after user input, to avoid losing error messages
        if (command != "undo" && command != "quit")
        {
            system("cls");
        }

        if (gameMode == 1)
        {
            cout << "Welcome to Chess (Player vs. Player)!" << endl;
        }
        else
        {
            cout << "Welcome to Chess (Player vs. AI)!" << endl;
        }

        chessBoard.printBoard();

        // AI's turn (Player 2 in Player vs AI mode)
        if (gameMode == 2 && currentPlayer == 2)
        {
            cout << "AI is making its move..." << endl;

            // Calculate the AI's move
            Move aiMove = chessBoard.calculateAIMove();

            // Validate the AI's move (safety check)
            if (!chessBoard.movePiece(aiMove.startX, aiMove.startY, aiMove.endX, aiMove.endY))
            {
                cerr << "AI attempted an invalid move. Ending AI's turn." << endl;
                continue; // Skip to the next iteration to avoid breaking the game flow
            }

            // Update the board state and switch turn
            cout << "AI moved piece from ("
                 << aiMove.startX << ", " << aiMove.startY << ") to ("
                 << aiMove.endX << ", " << aiMove.endY << ")" << endl;

            currentPlayer = 1; // Switch back to Player 1
            continue;
        }

        cout << "Player " << currentPlayer << "'s turn. Enter your move (e.g., e2 e4), 'undo' to undo last move, or 'quit' to exit: ";
        cin >> command;

        if (command == "quit")
        {
            cout << "Game ended. Thanks for playing!" << endl;
            break;
        }

        else if (command == "undo")
        {
            chessBoard.undoMove();                        // Just call the undoMove function directly
            currentPlayer = (currentPlayer == 1) ? 2 : 1; // Switch the player back after undo
            cout << "Last move has been undone!" << endl;
            continue;
        }

        cin >> target;

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
    }

    return 0;
}
