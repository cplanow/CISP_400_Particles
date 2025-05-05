#include "Matrices.h"

namespace Matrices {

Matrix::Matrix(int _rows, int _cols) : rows(_rows), cols(_cols) {
    a.resize(_rows);
    for (int i = 0; i < _rows; i++) {
        a[i].resize(_cols, 0);
    }
}

RotationMatrix::RotationMatrix(double theta) : Matrix(2, 2) {
    a[0][0] = cos(theta);
    a[0][1] = -sin(theta);
    a[1][0] = sin(theta);
    a[1][1] = cos(theta);
}

ScalingMatrix::ScalingMatrix(double scale) : Matrix(2, 2) {
    a[0][0] = scale;
    a[0][1] = 0;
    a[1][0] = 0;
    a[1][1] = scale;
}

TranslationMatrix::TranslationMatrix(double xShift, double yShift, int nCols) : Matrix(2, nCols) {
    for (int j = 0; j < nCols; j++) {
        a[0][j] = xShift;
        a[1][j] = yShift;
    }
}

Matrix operator+(const Matrix& a, const Matrix& b) {
    if (a.getRows() != b.getRows() || a.getCols() != b.getCols()) {
        throw runtime_error("Matrix dimensions must match for addition");
    }
    Matrix result(a.getRows(), a.getCols());
    for (int i = 0; i < a.getRows(); i++) {
        for (int j = 0; j < a.getCols(); j++) {
            result(i, j) = a(i, j) + b(i, j);
        }
    }
    return result;
}

Matrix operator*(const Matrix& a, const Matrix& b) {
    if (a.getCols() != b.getRows()) {
        throw runtime_error("Invalid matrix dimensions for multiplication");
    }
    Matrix result(a.getRows(), b.getCols());
    for (int i = 0; i < a.getRows(); i++) {
        for (int j = 0; j < b.getCols(); j++) {
            for (int k = 0; k < a.getCols(); k++) {
                result(i, j) += a(i, k) * b(k, j);
            }
        }
    }
    return result;
}

bool operator==(const Matrix& a, const Matrix& b) {
    if (a.getRows() != b.getRows() || a.getCols() != b.getCols()) {
        return false;
    }
    for (int i = 0; i < a.getRows(); i++) {
        for (int j = 0; j < a.getCols(); j++) {
            if (a(i, j) != b(i, j)) {
                return false;
            }
        }
    }
    return true;
}

bool operator!=(const Matrix& a, const Matrix& b) {
    return !(a == b);
}

ostream& operator<<(ostream& os, const Matrix& a) {
    for (int i = 0; i < a.getRows(); i++) {
        for (int j = 0; j < a.getCols(); j++) {
            os << a(i, j) << ' ';
        }
        os << '\n';
    }
    return os;
}

} // namespace Matrices