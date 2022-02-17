#include <iostream>
#include <ctime>

using namespace std;

#define CHECKS_COUNT 3

enum SolutionCount {
    zero = 0,
    one = 1,
    infinity = -1,
};

float** enterMatrix(int&, int&);
float** createMatrix(int, int);
void freeMatrix(float**, int);
void copyMatrix(float**, float**, int, int);
void printMatrix(float**, int, int);
void swapRows(float**, int, int, int);
void printGeneralSolution(float**, int, int);
void printEquations(float**, int, int);
void printSpecificSolution(float*, int);

// steps
void toUpperTriangularView(float**, int, int);
int searchSolutions(float**, int, int, float**);
void solving(float**, int, int, float*);
void checkGeneralSolution(float**, int, int, float**);
void checkSpecificSolution(float**, int, int, float*);

int main() {
    srand(time(0));
    setlocale(LC_ALL, "Rus");
    int rows, cols;


    float** matrix = enterMatrix(rows, cols);
    float** sourceMatrix = createMatrix(rows, cols);
    float** generalSolution = createMatrix(cols - 1, cols);
    float* sol = new float[cols - 1]{};

    copyMatrix(matrix, sourceMatrix, rows, cols);

    cout << endl << "Исходная матрица" << endl;
    printMatrix(matrix, rows, cols);

    cout << endl << "Шаг 1 (приведение к верхетреугольному виду)..." << endl;
    toUpperTriangularView(matrix, rows, cols);
    cout << "Матрица после первого шага:" << endl;
    printMatrix(matrix, rows, cols);

    cout << endl << "Шаг 2 (поиск решений)..." << endl;
    switch (searchSolutions(matrix, rows, cols, generalSolution)) {
    case SolutionCount(zero):
        cout << "Решений нет" << endl;
        break;
    case SolutionCount(one):
        solving(generalSolution, rows, cols, sol);
        cout << "Система имеет единственное решение" << endl;
        cout << "Общее решение: " << endl;
        printGeneralSolution(generalSolution, cols - 1, cols);
        cout << "Частное решение: " << endl;
        printSpecificSolution(sol, cols - 1);
        cout << endl;

        cout << endl << "Шаг 4 (проверка)..." << endl;
        checkSpecificSolution(sourceMatrix, rows, cols, sol);
        break;
    case SolutionCount(infinity):
        cout << "Система имеет бесконечное множество решений" << endl;
        printGeneralSolution(generalSolution, cols - 1, cols);

        cout << endl << "Шаг 4 (проверка)..." << endl;
        checkGeneralSolution(sourceMatrix, rows, cols, generalSolution);
        break;
    }

    freeMatrix(matrix, rows);
    freeMatrix(sourceMatrix, rows);
    delete[] generalSolution;
    delete[] sol;
    return 0;
}

float** enterMatrix(int& rows, int& cols) {
    cout << "Введите размерность расширенной матрицы(m, n): ";
    cin >> rows >> cols;
    while (rows < 1 || cols < 1 || cin.fail()) {
        cout << "Размерность введена неверно" << endl;
        cout << "Введите заново(m > 0, n > 0): ";
        cin >> rows >> cols;
        if (cin.fail()) continue;
    }
    cout << "Полученная рамерность: " << rows << "x" << cols << endl << endl;

    float** newMatrix = createMatrix(rows, cols);

    cout << "Заполните строки(включая свободные коэф.)" << endl;
    for (int r = 0; r < rows; r++) {
        cout << "Введите " << r + 1 << " строку матрицы(разделяя эл-ты пробелом): ";
        for (int c = 0; c < cols; c++) cin >> newMatrix[r][c];
    }

    return newMatrix;
}

float** createMatrix(int rows, int cols) {
    float** newMatrix = new float* [rows] {};
    for (int i = 0; i < rows; i++) newMatrix[i] = new float[cols] {};
    return newMatrix;
}

void freeMatrix(float** matrix, int rows) {
    for (int i = 0; i < rows; i++) delete[] matrix[i];
    delete[] matrix;
}

void copyMatrix(float** source, float** dest, int rows, int cols) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            dest[r][c] = source[r][c];
        }
    }
}

void printMatrix(float** matrix, int rows, int cols) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            printf("%.2f\t", matrix[r][c]);
        }
        cout << endl;
    }
}

void swapRows(float** matrix, int cols, int row1, int row2) {
    for (int col = 0; col < cols; col++) {
        float temp = matrix[row1][col];
        matrix[row1][col] = matrix[row2][col];
        matrix[row2][col] = temp;
    }
}


void printGeneralSolution(float** generalSolution, int rows, int cols) {
    for (int x = 0; x < rows; x++) {
        printf("x%d = ", x + 1);
        bool allZeros = true;
        bool firstWritten = false;
        for (int col = 0; col < cols - 1; col++) {
            if (generalSolution[x][col] == 0.0 || x == col) continue;
            if (allZeros) allZeros = false;
            if (generalSolution[x][col] < 0) printf("- ");
            if (firstWritten) {
                if (generalSolution[x][col] >= 0) printf("+ ");
            }
            if (!firstWritten) firstWritten = true;
            if (abs(generalSolution[x][col]) != 1.0) printf("%.2f*", abs(generalSolution[x][col]));
            cout << "x" << col + 1 << " ";
        }
        if (allZeros && generalSolution[x][cols - 1] == 0.0 && generalSolution[x][x] != 1) {
            printf("t, t c- R\n");
        }
        else {
            if (generalSolution[x][cols - 1] < 0) printf("- ");
            if (firstWritten) {
                if (generalSolution[x][cols - 1] >= 0) printf("+ ");
            }
            printf("%.2f\n", abs(generalSolution[x][cols - 1]));
        }
    }
}

void printEquations(float** matrix, int rows, int cols) {
    for (int row = rows - 1; row >= 0; row--) {
        bool allZeros = true;
        bool firstWritten = false;
        for (int col = 0; col < cols - 1; col++) {
            if (matrix[row][col] != 0.0) {
                if (allZeros) allZeros = false;
                if (firstWritten) {
                    if (matrix[row][col] >= 0) printf("+ ");
                    else printf("- ");
                }
                if (!firstWritten) firstWritten = true;
                if (matrix[row][col] != 1.0) cout << printf("%.2f*", abs(matrix[row][col]));
                printf("x%d ", col + 1);
            }
        }
        if (allZeros) cout << "0 ";
        printf("= %.2f\n", matrix[row][cols - 1]);
    }
}

void printSpecificSolution(float* sol, int size) {
    cout << "(";
    for (int i = 0; i < size; i++) printf("%.2f, ", sol[i]);
    cout << ")";
}

void toUpperTriangularView(float** matrix, int rows, int cols) {
    for (int globalCol = 0, globalRow = 0; globalCol < cols && globalRow < rows; globalCol++, globalRow++) {
        float firstEl = matrix[globalRow][globalCol];
        if (firstEl == 0.0) {
            bool found = false;
            for (int row = globalRow; row < rows; row++) {
                if (matrix[row][globalCol] != 0) {
                    found = true;
                    swapRows(matrix, cols, globalRow, row);
                    firstEl = matrix[globalRow][globalCol];
                    break;
                }
            }
            if (!found) {
                globalRow--;
                continue;
            }
        }

        for (int col = globalCol; col < cols; col++) {
            matrix[globalRow][col] /= firstEl;
        }

        if (globalRow == rows - 1) {
            continue;
        }

        float* rowNow = new float[cols - globalCol];
        for (int col = 0; col < cols - globalCol; col++) rowNow[col] = matrix[globalRow][col + globalCol];

        for (int row = globalRow + 1; row < rows; row++) {
            float coef = matrix[row][globalCol];
            for (int col = globalCol; col < cols; col++) {
                matrix[row][col] -= (rowNow[col - globalCol] * coef);
            }
        }

        delete[] rowNow;
        printMatrix(matrix, rows, cols);
        printf("\n");
    }
}

int searchSolutions(float** matrix, int rows, int cols, float** generalSolution) {
    int result = SolutionCount(one);
    cout << "Составление уравнений..." << endl;
    printEquations(matrix, rows, cols);
    cout << endl;

    bool haveInfinity = false;
    bool haveError = false;

    for (int row = 0; row < rows; row++) {
        float rightPart = matrix[row][cols - 1];
        int nonZeroCount = 0;
        int nowX = cols - 1;

        for (int col = 0; col < cols - 1; col++) {
            if (matrix[row][col] != 0) {
                nonZeroCount++;
            }
            if (nowX == cols - 1 && matrix[row][col] == 1) nowX = col;
        }

        if (nowX != cols - 1) {
            for (int col = 0; col < cols - 1; col++) {
                generalSolution[nowX][col] = -matrix[row][col];
            }
            generalSolution[nowX][cols - 1] = matrix[row][cols - 1];
            generalSolution[nowX][nowX] = 1;
        }

        if (!nonZeroCount) {
            if (rightPart != 0) {
                haveError = true;
                result = SolutionCount(zero);
                break;
            }
            haveInfinity = true;
        }
    }

    for (int i = 0; i < cols - 1; i++) {
        if (generalSolution[i][i] == 0) {
            haveInfinity = true;
            break;
        }
    }

    if (haveInfinity) result = SolutionCount(infinity);
    if (haveError) result = SolutionCount(zero);
    return result;
}

void solving(float** generalSolution, int rows, int cols, float* sol) {
    for (int row = rows - 1; row >= 0; row--) {
        sol[row] = generalSolution[row][cols - 1];
        for (int i = cols - 2; i > row; i--) sol[row] += generalSolution[row][i] * sol[i];
    }
}

void checkGeneralSolution(float** matrix, int rows, int cols, float** generalSolution) {
    int freeCount = 0;
    for (int i = 0; i < rows; i++) if (generalSolution[i][i] == 0.0) freeCount++;
    int specSolIndex = 0;
    float** newSolution = createMatrix(cols - 1, cols);
    copyMatrix(generalSolution, newSolution, cols - 1, cols);
    for (int row = 0; row < cols - 1; row++) {
        if (newSolution[row][row] != 1.0) {
            float randomValue = (-10000 + (rand() % (10000 - (-10000) + 1))) / (float)100.0;
            newSolution[row][row] = 1.0;
            newSolution[row][cols - 1] = randomValue;
            printf("Пусть x%d = %.2f\n", row + 1, randomValue);
        }
    }

    printf("\nТогда:\n");

    float* sol = new float[cols - 1];
    solving(newSolution, cols - 1, cols, sol);
    for (int x = 0; x < cols - 1; x++) printf("x%d = %.2f\n", x + 1, sol[x]);
    printf("\nПроверка полученного решения:\n");
    checkSpecificSolution(matrix, rows, cols, sol);

    /*for (int i = 0; i < CHECKS_COUNT; i++) {
        for (int valueNum = 0; valueNum < freeCount; valueNum++) {
            randomValues[valueNum] = (-10000 + (rand() % ((10000 - (-10000)) + 1))) / 100;

        }


    }*/
    delete[] sol;
    freeMatrix(newSolution, cols - 1);
}

void checkSpecificSolution(float** sourceMatrix, int rows, int cols, float* sol) {
    printf("Подставим полученные значения ");
    printSpecificSolution(sol, cols - 1);
    printf(" в исходную матрицу\n");

    for (int row = 0; row < rows; row++) {
        float leftPart = 0.0;
        float rightPart = sourceMatrix[row][cols - 1];
        bool firstWritten = false;
        bool allZeros = true;
        for (int col = 0; col < cols - 1; col++) {
            if (allZeros && sourceMatrix[row][col] != 0.0) allZeros = false;
            if (firstWritten) {
                if (sourceMatrix[row][col] >= 0) printf("+ ");
                else printf("- ");
            }
            if (!firstWritten) firstWritten = true;
            printf("%.2f*%.2f ", abs(sourceMatrix[row][col]), sol[col]);
            leftPart += sourceMatrix[row][col] * sol[col];
        }
        if (allZeros) printf("0 ");
        printf("= %.2f\t |\t", sourceMatrix[row][cols - 1]);
        printf("%.2f = %.2f\t |\t", leftPart, rightPart);
        if (round(leftPart * 100) / 100 == round(rightPart * 100) / 100) printf("верно");
        else printf("неверно(");
        printf("\n");
    }
}