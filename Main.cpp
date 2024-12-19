#include <iostream>
#include "Board.h"
using namespace std;

int main()
{
    Board chessBoard;
    chessBoard.setupBoard();

    string input, target, command;
    int gameMode = 0;      // 1 for Player vs Player, 2 for Player vs AI
    int currentPlayer = 1; // 1 for Player 1 (White), 2 for Player 2 (Black)

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
            system("cls"); // Clear the screen for fresh display
        }

        cout << "Welcome to Chess (" << (gameMode == 1 ? "Player vs. Player" : "Player vs. AI") << ")!" << endl;
        chessBoard.printBoard();
if (gameMode == 2 && currentPlayer == 2) {
    cout << "AI is making its move..." << endl;

    Move aiMove = chessBoard.calculateAIMove();

    if (aiMove.startX == -1 && aiMove.startY == -1) {
        cout << "AI has no valid moves. The game is over!" << endl;

        // if (chessBoard.isKingInCheck(false)) {
        //     cout << "Checkmate! Player 1 wins!" << endl;
        // } else {
        //     cout << "Stalemate! The game is a draw." << endl;
        // }
        break;
    }

    if (chessBoard.movePiece(aiMove.startX, aiMove.startY, aiMove.endX, aiMove.endY)) {
        cout << "AI moved piece from (" << aiMove.startX << ", " << aiMove.startY
             << ") to (" << aiMove.endX << ", " << aiMove.endY << ")" << endl;

        currentPlayer = 1; // Switch to Player 1
    } else {
        cerr << "AI attempted an invalid move. This should not happen." << endl;
    }
}


        // Player's turn
        cout << "Player " << currentPlayer << "'s turn. Enter your move (e.g., e2 e4), 'undo' to undo last move, or 'quit' to exit: ";
        cin >> command;

        if (command == "quit")
        {
            cout << "Game ended. Thanks for playing!" << endl;
            break;
        }

        if (command == "undo")
        {
            chessBoard.undoMove(); // Undo the last move
            currentPlayer = (currentPlayer == 1) ? 2 : 1; // Switch back the player after undo
            cout << "Last move has been undone!" << endl;
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




//Testing123
        // // Check for game-ending conditions
        // if (chessBoard.isGameOver())
        // {
        //     cout << "Game over! Player " << (currentPlayer == 1 ? 2 : 1) << " wins!" << endl;
        //     break;
        // }
    }

    return 0;
}