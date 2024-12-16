#include <iostream>
#include <time.h>
#include <conio.h>
#include <cstdlib> // Include for rand()
using namespace std;

enum Direction {IDLE = 0, LEFT = 1, UPLEFT = 2, DOWNLEFT = 3, RIGHT = 4, UPRIGHT = 5, DOWNRIGHT = 6};

class Ball
{
private:
    int xPos, yPos;
    int initialX, initialY;
    Direction ballDirection;
public:
    Ball(int startX, int startY)
    {
        initialX = startX;
        initialY = startY;
        xPos = startX;
        yPos = startY;
        ballDirection = IDLE;
    }

    void Reset()
    {
        xPos = initialX;
        yPos = initialY;
        ballDirection = IDLE;
    }

    void setDirection(Direction dir)
    {
        ballDirection = dir;
    }

    void randomDirection()
    {
        ballDirection = (Direction)((rand() % 6) + 1);
    }

    inline int getX() const { return xPos; }
    inline int getY() const { return yPos; }
    inline Direction getDirection() const { return ballDirection; }

    void move()
    {
        switch(ballDirection)
        {
        case IDLE:
            break;
        case LEFT:
            xPos--;
            break;
        case RIGHT:
            xPos++;
            break;
        case UPLEFT:
            xPos--; yPos--;
            break;
        case DOWNLEFT:
            xPos--; yPos++;
            break;
        case UPRIGHT:
            xPos++; yPos--;
            break;
        case DOWNRIGHT:
            xPos++; yPos++;
            break;
        default:
            break;
        }
    }

    friend ostream & operator<< (ostream & out, const Ball & b)
    {
        out << "Ball [" << b.xPos << "," << b.yPos << "][" << b.ballDirection << "]";
        return out;
    }
};

class Paddle
{
private:
    int xPos, yPos;
    int initialX, initialY;
public:
    Paddle() : xPos(0), yPos(0) {}

    Paddle(int startX, int startY) : xPos(startX), yPos(startY)
    {
        initialX = startX;
        initialY = startY;
    }

    inline void Reset() { xPos = initialX; yPos = initialY; }

    inline int getX() const { return xPos; }
    inline int getY() const { return yPos; }

    inline void moveUp() { yPos--; }

    inline void moveDown() { yPos++; }

    friend ostream & operator<<(ostream & out, const Paddle & p)
    {
        out << "Paddle [" << p.xPos << "," << p.yPos << "]";
        return out;
    }
};

class GameManager
{
private:
    int width, height;
    int player1Score, player2Score;
    char player1Up, player1Down, player2Up, player2Down;
    bool isQuit;
    Ball *gameBall;
    Paddle *player1Paddle;
    Paddle *player2Paddle;

public:
    GameManager(int gameWidth, int gameHeight)
    {
        srand(time(NULL));
        isQuit = false;
        player1Up = 'w'; player2Up = 'i';
        player1Down = 's'; player2Down = 'k';
        player1Score = player2Score = 0;
        width = gameWidth;
        height = gameHeight;

        gameBall = new Ball(width / 2, height / 2);
        player1Paddle = new Paddle(1, height / 2 - 3);
        player2Paddle = new Paddle(width - 2, height / 2 - 3);
    }

    ~GameManager()
    {
        delete gameBall;
        delete player1Paddle;
        delete player2Paddle;
    }

    void incrementScore(Paddle *player)
    {
        if (player == player1Paddle)
            player1Score++;
        else if (player == player2Paddle)
            player2Score++;

        gameBall->Reset();
        player1Paddle->Reset();
        player2Paddle->Reset();
    }

    void drawField()
    {
        system("cls");

        for (int i = 0; i < width + 2; i++)
            cout << "\xB2";
        cout << endl;

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                int ballX = gameBall->getX();
                int ballY = gameBall->getY();
                int player1X = player1Paddle->getX();
                int player2X = player2Paddle->getX();
                int player1Y = player1Paddle->getY();
                int player2Y = player2Paddle->getY();

                if (j == 0)
                    cout << "\xB2";

                if (ballX == j && ballY == i)
                    cout << "O";
                else if (player1X == j && (player1Y <= i && player1Y + 3 >= i))
                    cout << "\xDB";
                else if (player2X == j && (player2Y <= i && player2Y + 3 >= i))
                    cout << "\xDB";
                else
                    cout << " ";

                if (j == width - 1)
                    cout << "\xB2";
            }
            cout << endl;
        }

        for (int i = 0; i < width + 2; i++)
            cout << "\xB2";
        cout << endl;

        cout << "Player 1: " << player1Score << endl;
        cout << "Player 2: " << player2Score << endl;
    }

    void processInput()
    {
        gameBall->move();

        if (_kbhit())
        {
            char key = _getch();
            int player1Y = player1Paddle->getY();
            int player2Y = player2Paddle->getY();

            if (key == player1Up && player1Y > 0)
                player1Paddle->moveUp();
            if (key == player1Down && player1Y + 4 < height)
                player1Paddle->moveDown();

            if (key == player2Up && player2Y > 0)
                player2Paddle->moveUp();
            if (key == player2Down && player2Y + 4 < height)
                player2Paddle->moveDown();

            if (gameBall->getDirection() == IDLE)
                gameBall->randomDirection();

            if (key == 'q')
                isQuit = true;
        }
    }

    void updateGame()
    {
        int ballX = gameBall->getX();
        int ballY = gameBall->getY();
        int player1X = player1Paddle->getX();
        int player2X = player2Paddle->getX();
        int player1Y = player1Paddle->getY();
        int player2Y = player2Paddle->getY();

        for (int i = 0; i < 4; i++)
            if (ballX == player1X + 1 && ballY == player1Y + i)
                gameBall->setDirection((Direction)((rand() % 3) + 4));

        for (int i = 0; i < 4; i++)
            if (ballX == player2X - 1 && ballY == player2Y + i)
                gameBall->setDirection((Direction)((rand() % 3) + 1));

        if (ballY == height - 1)
            gameBall->setDirection(ballY == DOWNRIGHT ? UPRIGHT : UPLEFT);

        if (ballY == 0)
            gameBall->setDirection(ballY == UPRIGHT ? DOWNRIGHT : DOWNLEFT);

        if (ballX == width - 1)
            incrementScore(player1Paddle);

        if (ballX == 0)
            incrementScore(player2Paddle);
    }

    void startGame()
    {
        while (!isQuit)
        {
            drawField();
            processInput();
            updateGame();
        }
    }
};

int main()
{
    GameManager pongGame(40, 20);
    pongGame.startGame();

    return 0;
}