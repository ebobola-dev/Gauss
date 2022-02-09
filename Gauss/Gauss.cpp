#include <iostream>

using namespace std;


int** createMatrix(int, int);
void freeMatrix(int**, int, int);
void copyMatrix(int**, int**, int, int);
void printMatrix(int**, int, int);
void subtractRowFromAll(int**, int, int, int, int);

void step1(int**, int, int);

int main() {
    setlocale(LC_ALL, "Rus");
    int row, col;

    cout << "Введите размерность матрицы(m, n)(Включая столбец свободных коэф.): ";
    cin >> row >> col;
    while (row < 1 || col < 1 || cin.fail()) {
        cout << "Размерность введена неверно" << endl;
        cout << "Введите заново(m > 0, n > 0): ";
        cin >> row >> col;
        if (cin.fail()) continue;
    }
    cout << "Полученная рамерность: " << row << "x" << col << endl << endl;

    int** matrix = createMatrix(row, col);
    int** sourceMatrix = createMatrix(row, col);

    cout << "Заполните строки(Включая свободные коэф.)" << endl;
    for (int r = 0; r < row; r++) {
        cout << "Введите " << r + 1 << " строку матрицы(пример: 1 -1 4 0): ";
        for (int c = 0; c < col; c++) cin >> matrix[r][c];
    }
    copyMatrix(matrix, sourceMatrix, row, col);

    cout << endl <<  "Полученная матрица" << endl;
    printMatrix(matrix, row, col);

    step1(matrix, row, col);
    cout << endl << "Матрица после первого шага" << endl;
    printMatrix(matrix, row, col);

    freeMatrix(matrix, row, col);
    freeMatrix(sourceMatrix, row, col);
    return 0;
}

int** createMatrix(int row, int col) {
    int** newMatrix = new int* [row];
    for (int i = 0; i < row; i++) newMatrix[i] = new int[col];
    return newMatrix;
}

void freeMatrix(int** matrix, int row, int col) {
    for (int i = 0; i < row; i++) delete[] matrix[i];
    delete[] matrix;
}

void copyMatrix(int** source, int** dest, int row, int col) {
    for (int r = 0; r < row; r++) {
        for (int c = 0; c < col; c++) {
            dest[r][c] = source[r][c];
        }
    }
}

void printMatrix(int** matrix, int row, int col) {
    for (int r = 0; r < row; r++) {
        for (int c = 0; c < col; c++) {
            cout << matrix[r][c] << " ";
        }
        cout << endl;
    }
}

void subtractRowFromAll(int** matrix, int row, int col, int rowNumber, int coef) {
    int* subtractedRow = new int[col];
    for (int c = 0; c < col; c++) subtractedRow[c] = matrix[rowNumber][c];

    for (int r = 0; r < row; r++) {
        for (int c = 0; c < col; c++) {
            //printf("substract: matrix[%d][%d](%d) -= matrix[%d][%d](%d) * %d\n", r, c, matrix[r][c], rowNumber, c, matrix[rowNumber][c], coef);
            matrix[r][c] -= subtractedRow[c] * coef;
        }
    }

    delete[] subtractedRow;
}

void step1(int** matrix, int row, int col) {
    int firstCol = -1;
    for (int r = 0; r < row; r++) {
        if (matrix[r][0] != 0) {
            firstCol = r; break;
        }
    }
    if (firstCol != -1) {
        subtractRowFromAll(matrix, row, col, firstCol, matrix[firstCol][0]);

        for (int r = 0; r < row; r++) {
            if (matrix[r][1] != 0) {
                firstCol = r; break;
            }
        }

        if (firstCol != -1) {
            subtractRowFromAll(matrix, row, col, firstCol, matrix[firstCol][0]);
        }
    }
}