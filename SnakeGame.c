#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define WIDTH 40
#define HEIGHT 20

int x, y, fruitX, fruitY, score, nTail, gameOver, sleepTime, isPaused;
int tailX[100], tailY[100];
char levelName[20];
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN } dir;

void HideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void Setup() {
    srand(time(NULL));
    gameOver = 0;
    isPaused = 0;
    dir = STOP;
    x = WIDTH / 2;
    y = HEIGHT / 2;
    fruitX = rand() % WIDTH;
    fruitY = rand() % HEIGHT;
    score = 0;
    nTail = 0;
}

void Draw() {
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    // LinkedIn-ready Header
    printf(">> NAHPI EEE PROJECT | Developer: Kuete Jibran <<\n");
    printf("Level: %-7s | Score: %-4d | Status: %s   \n",
            levelName, score, isPaused ? "[PAUSED]" : "RUNNING ");

    for (int i = 0; i < WIDTH + 2; i++) printf("#");
    printf("\n");

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0) printf("#");

            if (i == y && j == x) printf("O"); // Head
            else if (i == fruitY && j == fruitX) printf("*"); // Fruit
            else {
                int isTail = 0;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        printf("o"); // Tail segments
                        isTail = 1;
                        break;
                    }
                }
                if (!isTail) printf(" ");
            }
            if (j == WIDTH - 1) printf("#");
        }
        printf("\n");
    }
    for (int i = 0; i < WIDTH + 2; i++) printf("#");
    printf("\n[P] Pause/Resume | [X] Quit Game               \n");
}

void Input() {
    if (_kbhit()) {
        char key = _getch();
        switch (key) {
            case 'a': if(dir != RIGHT && !isPaused) dir = LEFT; break;
            case 'd': if(dir != LEFT && !isPaused) dir = RIGHT; break;
            case 'w': if(dir != DOWN && !isPaused) dir = UP; break;
            case 's': if(dir != UP && !isPaused) dir = DOWN; break;
            case 'p':
            case 'P': isPaused = !isPaused; break; // Toggle Pause
            case 'x': gameOver = 1; break;
        }
    }
}

void Logic() {
    if (isPaused) return; // Freeze logic during pause

    int prevX = tailX[0], prevY = tailY[0], prev2X, prev2Y;
    tailX[0] = x; tailY[0] = y;
    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i]; prev2Y = tailY[i];
        tailX[i] = prevX; tailY[i] = prevY;
        prevX = prev2X; prevY = prev2Y;
    }

    switch (dir) {
        case LEFT:  x--; break;
        case RIGHT: x++; break;
        case UP:    y--; break;
        case DOWN:  y++; break;
        default: break;
    }

    // Collision Detection
    if (x >= WIDTH || x < 0 || y >= HEIGHT || y < 0) gameOver = 1;
    for (int i = 0; i < nTail; i++) if (tailX[i] == x && tailY[i] == y) gameOver = 1;

    // Fruit Consumption
    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % WIDTH;
        fruitY = rand() % HEIGHT;
        nTail++;
    }
}

void Menu() {
    system("cls");
    printf("====================================\n");
    printf("      WELCOME TO SNAKE GAME        \n");
    printf("====================================\n");
    printf("SELECT DIFFICULTY:\n");
    printf("1. Easy\n2. Normal\n3. Hard\n\nChoice: ");

    char choice = _getch();
    switch(choice) {
        case '1': sleepTime = 100; sprintf(levelName, "EASY"); break;
        case '2': sleepTime = 50;  sprintf(levelName, "NORMAL"); break;
        case '3': sleepTime = 20;  sprintf(levelName, "HARD"); break;
        default:  sleepTime = 50;  sprintf(levelName, "NORMAL"); break;
    }
    system("cls");
}

int main() {
    HideCursor();
    int keepPlaying = 1;

    while (keepPlaying) {
        Menu();
        Setup();

        while (!gameOver) {
            Draw();
            Input();
            Logic();
            Sleep(sleepTime);
        }

        system("cls");
        printf("\n\n   GAME OVER!\n");
        printf("   Level: %s | Final Score: %d\n", levelName, score);
        printf("   ----------------------------\n");
        printf("   Play again? (Y/N): ");

        char retry = _getch();
        if (retry == 'n' || retry == 'N') {
            keepPlaying = 0;
            printf("\n   Project by Kuete Jibran (NAHPI EEE). Goodbye!\n");
            Sleep(2000);
        }
    }
    return 0;
}
