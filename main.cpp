#include <iostream>
#include "Board.h"
using namespace std;

int main()
{
    Board chessBoard;
    chessBoard.setupBoard();

    string input, target, command;
    int currentPlayer = 1; // 1 for Player 1 (White), 2 for Player 2 (Black)
    bool firstMoveMade = false; // Track if the first move has been made

    while (true)
    {


        cout << "Welcome to Chess (Player vs. Player)!" << endl;
        chessBoard.printBoard();

        // Display check warning
        if (chessBoard.isKingInCheck(currentPlayer == 1))
        {
            cout << "Player " << currentPlayer << "'s king is in check!" << endl;
        }

        cout << "Player " << currentPlayer << "'s turn. Enter your move (e.g., e2 e4), 'undo' to undo last move, 'redo' to redo the undone move, or 'quit' to exit: ";
        cin >> command;

        if (command == "quit")
        {
            cout << "Game ended. Thanks for playing!" << endl;
            break;
        }
        else if (command == "undo")
        {
            if (!firstMoveMade)
            {
                cout << "Undo is not available as no move has been made yet!" << endl;
                continue;
            }
            chessBoard.undoMove();
            currentPlayer = (currentPlayer == 1) ? 2 : 1; // Switch turns back
            cout << "Last move has been undone!" << endl;
            continue;
        }
        else if (command == "redo")
        {
            if (!firstMoveMade)
            {
                cout << "Redo is not available as no move has been made yet!" << endl;
                continue;
            }
            chessBoard.redoMove();
            currentPlayer = (currentPlayer == 1) ? 2 : 1; // Switch turns forward
            cout << "Move has been redone!" << endl;
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
                firstMoveMade = true; // Mark that the first move has been made
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
