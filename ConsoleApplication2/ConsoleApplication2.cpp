#include <Windows.h> // включаем заголовочный файл Windows API
#include <iostream> // включаем заголовочный файл ввода/вывода
#include <conio.h> // включаем заголовочный файл консольного ввода/вывода
#include <cstdlib> // включаем заголовочный файл стандартной библиотеки
#include <ctime> // включаем заголовочный файл времени
#include <string> // включаем заголовочный файл строк

using namespace std; // используем стандартное пространство имен

const int WINDOW_WIDTH = 80; // ширина окна
const int WINDOW_HEIGHT = 25; // высота окна
int PADDLE_WIDTH = 40; // ширина платформы
const int PADDLE_HEIGHT = 2; // высота платформы
const int BLOCK_WIDTH = 8; // ширина блока
const int BLOCK_HEIGHT = 2; // высота блока
const int BLOCK_ROWS = 5; // количество строк блоков
const int BLOCK_COLS = 10; // количество столбцов блоков
const int PLATFORM_SPEED = 5; // скорость платформы

int ballX = WINDOW_WIDTH / 2; // начальная позиция шара по x
int ballY = WINDOW_HEIGHT - 3; // начальная позиция шара по y
int ballDirX = -1; // начальное направление шара по x
int ballDirY = -1; // начальное направление шара по y
int paddleX = WINDOW_WIDTH / 2 - PADDLE_WIDTH / 2; // начальная позиция платформы по x
int paddleY = WINDOW_HEIGHT - 2; // начальная позиция платформы по y
int blocks[BLOCK_ROWS][BLOCK_COLS] = { 0 }; // матрица блоков

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // получаем handle консоли

void drawWindow() { // рисуем окно игры
    COORD cursorPos = { 0, 0 }; // устанавливаем позицию курсора
    SetConsoleCursorPosition(hConsole, cursorPos); // перемещаем курсор в левый верхний угол
    for (int y = 0; y < WINDOW_HEIGHT; y++) { // проходим по строкам окна
        for (int x = 0; x < WINDOW_WIDTH; x++) { // проходим по столбцам окна
            if (x == 0 || x == WINDOW_WIDTH - 1 || y == 0 || y == WINDOW_HEIGHT - 1) { // рисуем рамку окна
                cout << '|';
            }
            else if (x >= paddleX && x < paddleX + PADDLE_WIDTH && y == paddleY) { // рисуем платформу
                cout << '=';
            }
            else if (x >= ballX && x < ballX + 1 && y == ballY) { // рисуем шар
                cout << 'O';
            }
            else { // рисуем блоки
                bool found = false;
                for (int i = 0; i < BLOCK_ROWS; i++) {
                    for (int j = 0; j < BLOCK_COLS; j++) {
                        if (blocks[i][j] && x >= j * BLOCK_WIDTH && x < j * BLOCK_WIDTH + BLOCK_WIDTH && y >= i * BLOCK_HEIGHT && y < i * BLOCK_HEIGHT + BLOCK_HEIGHT) {
                            cout << '#';
                            found = true;
                            break;
                        }
                    }
                    if (found) break;
                }
                if (!found) cout << ' '; // рисуем пустое пространство
            }
        }
        cout << endl; // переходим на следующую строку
    }
}

void moveBall() { // обновляем позицию шара и его направление
    ballX += ballDirX;
    ballY += ballDirY;
    if (ballX <= 1 || ballX >= WINDOW_WIDTH - 2) ballDirX *= -1; // отскок от левой/правой стены
    if (ballY <= 2) ballDirY *= -1; // отскок от верхней стены
    if (ballY >= paddleY - 1 && ballX >= paddleX - 1 && ballX < paddleX + 1 + PADDLE_WIDTH) ballDirY *= -1; // отскок от платформы
    for (int i = 0; i < BLOCK_ROWS; i++) {
        for (int j = 0; j < BLOCK_COLS; j++) {
            if (blocks[i][j] && ballX >= j * BLOCK_WIDTH && ballX < j * BLOCK_WIDTH + BLOCK_WIDTH && ballY >= i * BLOCK_HEIGHT && ballY < i * BLOCK_HEIGHT + BLOCK_HEIGHT) {
                blocks[i][j] = 0; // удаляем блок
                ballDirY *= -1; // отскок от блока
            }
        }
    }
    if (ballY >= WINDOW_HEIGHT - 2) { // игра окончена
        cout << "Игра окончена!" << endl;
        Sleep(2000);
        system("CLS");
        return;
    }
}

void movePaddle() { // обновляем позицию платформы
    if (_kbhit()) { // проверяем, есть ли ввод с клавиатуры
        char key = _getch();
        if (key == 'a' && paddleX > 1) paddleX -= PLATFORM_SPEED; // перемещаем платформу влево
        else if (key == 'd' && paddleX < WINDOW_WIDTH - PADDLE_WIDTH - 1) paddleX += PLATFORM_SPEED; // перемещаем платформу вправо
    }
}

void drawMenu() { // рисуем меню
    Sleep(100);
    system("CLS");
    COORD cursorPos = { 0, 0 };
    SetConsoleCursorPosition(hConsole, cursorPos);
    cout << "Меню:" << endl;
    cout << "1. Начать игру" << endl;
    cout << "2. Выход" << endl;
    cout << "Выберите пункт: ";
}

void drawDificulty() { // рисуем выбор сложности
    Sleep(100);
    system("CLS");
    COORD cursorPos = { 0, 0 };
    SetConsoleCursorPosition(hConsole, cursorPos);
    cout << "Сложность:" << endl;
    cout << "1. Легко" << endl;
    cout << "2. Нормально" << endl;
    cout << "3. Сложно" << endl;
    cout << "Выберите пункт: ";
}

int main() {
    setlocale(LC_ALL, "RUS"); // устанавливаем локаль на русский язык
    srand(time(0)); // инициализируем генератор случайных чисел
    while (true) {
        char choice; // переменная для хранения выбора пользователя
        drawMenu(); // рисуем меню
        cin >> choice; // считываем выбор пользователя
        if (choice == '1') { // если пользователь выбрал начать игру
            int difficulty = 100;
            drawDificulty();
            char new_choice;
            cin >> new_choice;
            if (new_choice == '1') {
                difficulty = 100;
                PADDLE_WIDTH = 40;
            } // легко

            else if (new_choice == '2') {
                difficulty = 20;
                PADDLE_WIDTH = 20;
            }// средне

            else {
                difficulty = 1;
                PADDLE_WIDTH = 6;
            }// сложно

            for (int i = 0; i < BLOCK_ROWS; i++) { // инициализируем матрицу блоков
                for (int j = 0; j < BLOCK_COLS; j++) {
                    blocks[i][j] = 1; // все блоки изначально существуют
                }
            }

            //Инициализация шарика и платформы в центре поля
            ballX = WINDOW_WIDTH / 2;
            ballY = WINDOW_HEIGHT - 3;
            ballDirX = -1;
            ballDirY = -1;
            paddleX = WINDOW_WIDTH / 2 - PADDLE_WIDTH / 2;
            paddleY = WINDOW_HEIGHT - 2;

            while (true) { // основной цикл игры
                drawWindow(); // рисуем окно игры
                movePaddle(); // обновляем позицию платформы
                moveBall(); // обновляем позицию шара и его направление
                Sleep(difficulty); // задержка для регулирования скорости игры
                if (ballY >= WINDOW_HEIGHT - 2) break;
            }
        }
        else if (choice == '2') { // если пользователь выбрал выйти
            exit(0); // завершаем программу
        }
    }
    return 0; // завершаем программу
}