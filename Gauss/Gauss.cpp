#include <iostream>
#include <cmath>

using namespace std;

#define DECIMAL_PLACES 2

float** createMatrix(int, int);
void freeMatrix(float**, int);
void copyMatrix(float**, float**, int, int);
void printMatrix(float**, int, int);
void swapRows(float**, int, int, int);
float round_(float);
void printSolutions(float*, int);

// steps
void toUpperTriangularView(float**, int, int);
int searchSolutions(float**, int, int, float*);

int main() {
    setlocale(LC_ALL, "Rus");
    int rows, cols;

    cout << "Введите размерность матрицы(m, n)(включая столбец свободных коэф.): ";
    cin >> rows >> cols;
    while (rows < 1 || cols < 1 || cin.fail()) {
        cout << "Размерность введена неверно" << endl;
        cout << "Введите заново(m > 0, n > 0): ";
        cin >> rows >> cols;
        if (cin.fail()) continue;
    }
    cout << "Полученная рамерность: " << rows << "x" << cols << endl << endl;

    float** matrix = createMatrix(rows, cols);
    float** sourceMatrix = createMatrix(rows, cols);
    float* solution = new float[cols];

    cout << "Заполните строки(включая свободные коэф.)" << endl;
    for (int r = 0; r < rows; r++) {
        cout << "Введите " << r + 1 << " строку матрицы(разделяя эл-ты пробелом): ";
        for (int c = 0; c < cols; c++) cin >> matrix[r][c];
    }
    copyMatrix(matrix, sourceMatrix, rows, cols);

    cout << endl <<  "Полученная матрица" << endl;
    printMatrix(matrix, rows, cols);

    cout << endl << "Шаг 1 (приведение к верхетреугольному виду)..." << endl;
    toUpperTriangularView(matrix, rows, cols);
    cout << "Матрица после первого шага:" << endl;
    printMatrix(matrix, rows, cols);

    cout << endl << "Шаг 2 (поиск решений)..." << endl;
    searchSolutions(matrix, rows, cols, solution);
    cout << "Решения:" << endl;
    printSolutions(solution, cols - 1);

    freeMatrix(matrix, rows);
    freeMatrix(sourceMatrix, rows);
    delete[] solution;
    return 0;
}

float** createMatrix(int rows, int cols) {
    float** newMatrix = new float* [rows];
    for (int i = 0; i < rows; i++) newMatrix[i] = new float[cols];
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
            cout << matrix[r][c] << "\t";
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

float round_(float value) {
    return round(value * pow(10, DECIMAL_PLACES)) / pow(10, DECIMAL_PLACES);
}

void printSolutions(float* solution, int size) {
    for (int i = 0; i < size; i++) printf("x%d = %.2f\n", i + 1, solution[i]);
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
                matrix[globalRow][col] = round_(matrix[globalRow][col] / firstEl);
            }

            if (globalRow == rows - 1) {
                continue;
            }

            float* rowNow = new float[cols - globalCol];
            for (int col = 0; col < cols - globalCol; col++) rowNow[col] = matrix[globalRow][col + globalCol];

            for (int row = globalRow + 1; row < rows; row++) {
                float coef = matrix[row][globalCol];
                for (int col = globalCol; col < cols; col++) {
                    matrix[row][col] = round_(matrix[row][col] - (rowNow[col - globalCol] * coef));
                }
            }

            delete[] rowNow;
    }
}

int searchSolutions(float** matrix, int rows, int cols, float* solution) {
    cout << "Составление уравнений..." << endl;
    int lastSolutionIndex = cols - 2;
    for (int row = rows - 1; row >= 0; row--) {
        for (int col = 0; col < cols - 1; col++) {
            if (matrix[row][col] != 0.0) {
                if (matrix[row][col] != 1.0) cout << abs(matrix[row][col]) << "*";
                cout << "x" << col + 1;
                if (col != cols - 2) {
                    if (matrix[row][col + 1] > 0) cout << " + ";
                    else cout << " - ";
                }
                if (col == cols - 2 && row == rows - 1) {
                    solution[lastSolutionIndex--] = matrix[row][cols - 1];
                }
            }
        }
        cout << " = " << matrix[row][cols - 1];
        if (row != rows - 1) {
            cout << " => x" << row + 1 << " = ";
            solution[lastSolutionIndex] = matrix[row][cols - 1];
            for (int i = cols - 2; i > lastSolutionIndex; i--) {
                solution[lastSolutionIndex] = round_(solution[lastSolutionIndex] - matrix[row][i] * solution[i]);
            }
            cout << solution[lastSolutionIndex--];
        }
        cout << endl;

    }
    return 0;
}