#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace std;

// Global Variables/Constants
const int canvasHeight = 22;
const int canvasLength = 82;
const int maxSnakeLength = 100;
bool playmode = false;
int snakeTailX[maxSnakeLength], snakeTailY[maxSnakeLength];
int snakeTailLen = 1;

// Set GameArea
char canvas[canvasHeight][canvasLength];

// Fruit structure to store the fruit's position and symbol
struct Fruit {
    int x;
    int y;
    char symbol;
};

Fruit fruit;

void setCanvas() {
    // Set Inside Area
    for (int i = 1; i < canvasHeight - 1; i++) {
        for (int j = 1; j < canvasLength - 1; j++) {
            canvas[i][j] = ' ';
        }
    }

    // Set Frame
    for (int i = 0; i < canvasHeight; i++) {
        canvas[i][0] = '\xB0';
        canvas[i][canvasLength - 1] = '\xB0';
    }

    for (int j = 0; j < canvasLength; j++) {
        canvas[0][j] = '\xB0';
        canvas[canvasHeight - 1][j] = '\xB0';
    }

    // Set initial position for the fruit
    fruit.x = rand() % (canvasLength - 2) + 1;
    fruit.y = rand() % (canvasHeight - 2) + 1;
    fruit.symbol = '*';
    canvas[fruit.y][fruit.x] = fruit.symbol;
}

void printCanvas() {
    for (int i = 0; i < canvasHeight; i++) {
        for (int j = 0; j < canvasLength; j++) {
            if (canvas[i][j] == '\xB0') {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6); // Brown
                cout << '\xB0';
            }
            else if (canvas[i][j] == '0') {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2); // Green for the snake's head
                cout << '0';
            }
            else if (canvas[i][j] == 'o') {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2); // Green for the snake's tail
                cout << 'o';
            }
            else if (canvas[i][j] == '*') {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4); // Red for the fruit
                cout << '*';
            }
            else {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // White
                cout << canvas[i][j];
            }
        }
        cout << endl;
    }
}


void printFruit() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4); // Red for the fruit

    // Check if the fruit is within the frame before printing
    if (fruit.x > 0 && fruit.x < canvasLength - 1 && fruit.y > 0 && fruit.y < canvasHeight - 1) {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { static_cast<SHORT>(fruit.x), static_cast<SHORT>(fruit.y) });
        cout << fruit.symbol;
    }

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // White
}

void gameMain() {
    snakeTailLen = 1;
    int scoremain = 1;
    int x = canvasLength / 2;
    int y = canvasHeight / 2;

    canvas[y][x] = '0';
    printCanvas();

    int direction = 77;
    int oppositeDirection;

    while (playmode == true) {
        oppositeDirection = (direction == 77) ? 75 : (direction == 75) ? 77 : (direction == 72) ? 80 : 72;

        if (_kbhit()) {
            int inputArrow = _getch();
            if (inputArrow == 72 || inputArrow == 80 || inputArrow == 75 || inputArrow == 77) {
                if (inputArrow != oppositeDirection) {
                    direction = inputArrow;
                }
            }
        }

        int nextX = x;
        int nextY = y;

        switch (direction) {
        case 72:  // Up
            nextY--;
            break;
        case 80:  // Down
            nextY++;
            break;
        case 75:  // Left
            nextX--;
            break;
        case 77:  // Right
            nextX++;
            break;
        }

        // Check for collisions
        if (nextX <= 0 || nextX >= canvasLength - 1 || nextY <= 0 || nextY >= canvasHeight - 1 || canvas[nextY][nextX] == 'o') {
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, canvasHeight + 1 });
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6); // Brown
            cout << "\xB0";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // White
            cout << " Game Over!, press any key to continue" << endl;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6); // Brown
            cout << "\xB0";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // White
            cout << " Your Score was: " << scoremain << endl;
            Sleep(1000);
            _getch();
            playmode = false;
        }
        else {
            // Update tail coordinates
            for (int i = snakeTailLen - 1; i > 0; i--) {
                snakeTailX[i] = snakeTailX[i - 1];
                snakeTailY[i] = snakeTailY[i - 1];
            }

            snakeTailX[0] = x;
            snakeTailY[0] = y;

            // Update head coordinates
            x = nextX;
            y = nextY;

            // Check for collision with the fruit
            if (y == fruit.y && x == fruit.x) {
                // Generate a new random position for the fruit that doesn't overlap with the snake
                do {
                    fruit.x = rand() % (canvasLength - 2) + 1;
                    fruit.y = rand() % (canvasHeight - 2) + 1;
                } while (canvas[fruit.y][fruit.x] == '0' || canvas[fruit.y][fruit.x] == 'o');
                scoremain++;
                snakeTailLen++;
                if (snakeTailLen > maxSnakeLength) {
                    snakeTailLen = maxSnakeLength;
                }

                canvas[fruit.y][fruit.x] = fruit.symbol;
            }


            // Update the canvas with the snake's head
            canvas[y][x] = '0';

            // Update the canvas with the snake's tail
            for (int i = 0; i < snakeTailLen; i++) {
                canvas[snakeTailY[i]][snakeTailX[i]] = 'o';
            }

            // Erase the tail from the old position
            if (snakeTailLen > 0) {
                canvas[snakeTailY[snakeTailLen - 1]][snakeTailX[snakeTailLen - 1]] = ' ';
            }

            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });
            printCanvas();
            printFruit();
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, canvasHeight + 1 });
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6); // Brown
            cout << "\xB0";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // White
            cout << " Your Score is: " << scoremain << endl;
            Sleep(50);

            // Reset frame elements after updating the fruit
            for (int i = 0; i < canvasHeight; i++) {
                canvas[i][0] = '\xB0';
                canvas[i][canvasLength - 1] = '\xB0';
            }

            for (int j = 0; j < canvasLength; j++) {
                canvas[0][j] = '\xB0';
                canvas[canvasHeight - 1][j] = '\xB0';
            }
        }
    }
}

void artGameMenu() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5); // Purple
    cout << "" << endl;
    cout << "#   #                     ##                     #                        ##               #                  ##" << endl;
    cout << "#   #         #          #  #               #    #                       #  #              #                 #  #" << endl;
    cout << " # #    ##   ####        #  #  ###    ##   ####  ###    ##   # ##        #     ###    ###  #  #   ##         #      ### ## #    ##" << endl;
    cout << "  #    #  #   #          ####  #  #  #  #   #    #  #  #  #  ##           ##   #  #  #  #  # #   #  #        # ##  #  # # # #  #  #" << endl;
    cout << "  #    ####   #          #  #  #  #  #  #   #    #  #  ####  #              #  #  #  #  #  ##    ####        #  #  #  # # # #  ####" << endl;
    cout << "  #    #      #          #  #  #  #  #  #   #    #  #  #     #           #  #  #  #  # ##  # #   #           #  #  # ## # # #  #" << endl;
    cout << "  #     ##     ##        #  #  #  #   ##     ##  #  #   ##   #            ##   #  #   # #  #  #   ##          ###   # # #   #   ##" << endl;


    cout << endl;
    cout << "Snake Game by Manav Marmat";
    cout << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // White
}

int main() {
    const int numOptions = 2;
    int selectedOption = 0;

    while (true) {
        system("cls"); // clear the console
        artGameMenu();
        // Display menu options
        for (int i = 0; i < numOptions; ++i) {
            if (i == selectedOption) {
                cout << ">> ";
            }
            else {
                cout << "   ";
            }

            // Display menu option
            if (i == 0) {
                cout << "Play" << endl;
            }
            else if (i == 1) {
                cout << "Exit" << endl;
            }
        }

        // Wait for user input
        int key = _getch();

        // Handle user input
        switch (key) {
        case 72:
            // Move selection up
            selectedOption = (selectedOption - 1 + numOptions) % numOptions;
            break;

        case 80:
            // Move selection down
            selectedOption = (selectedOption + 1) % numOptions;
            break;

        case 13: // Enter key
            // Perform action based on the selected option
            if (selectedOption == 0) {
                playmode = true;
                system("cls"); // clear the console
                setCanvas();
                gameMain();
            }
            else if (selectedOption == 1) {
                return 1;
            }

        default:
            // Ignore other keys
            break;
        }
    }
    return 0;
}