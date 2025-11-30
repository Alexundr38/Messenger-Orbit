//
// Created by magofrays on 11/30/25.
//


#include "Mat3d.h"

#include <cmath>
#include <algorithm>

struct Vec3d;
// Конструкторы
Mat3d::Mat3d() {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            m[i][j] = 0.0;
}

Mat3d::Mat3d(double m00, double m01, double m02,
             double m10, double m11, double m12,
             double m20, double m21, double m22) {
    m[0][0] = m00; m[0][1] = m01; m[0][2] = m02;
    m[1][0] = m10; m[1][1] = m11; m[1][2] = m12;
    m[2][0] = m20; m[2][1] = m21; m[2][2] = m22;
}

Mat3d::Mat3d(const Mat3d& other) {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            m[i][j] = other.m[i][j];
}

// Статические методы
Mat3d Mat3d::Zero() {
    return Mat3d();
}

Mat3d Mat3d::Identity() {
    return Mat3d(1, 0, 0,
                 0, 1, 0,
                 0, 0, 1);
}

Mat3d Mat3d::Diagonal(double d0, double d1, double d2) {
    return Mat3d(d0, 0,  0,
                 0,  d1, 0,
                 0,  0,  d2);
}

// Операторы доступа
double& Mat3d::operator()(int i, int j) {
    return m[i][j];
}

const double& Mat3d::operator()(int i, int j) const {
    return m[i][j];
}

double* Mat3d::operator[](int i) {
    return m[i];
}

const double* Mat3d::operator[](int i) const {
    return m[i];
}

// Арифметические операторы
Mat3d Mat3d::operator+(const Mat3d& other) const {
    Mat3d result;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            result.m[i][j] = m[i][j] + other.m[i][j];
    return result;
}

Mat3d Mat3d::operator-(const Mat3d& other) const {
    Mat3d result;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            result.m[i][j] = m[i][j] - other.m[i][j];
    return result;
}

Mat3d Mat3d::operator*(const Mat3d& other) const {
    Mat3d result;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result.m[i][j] = 0.0;
            for (int k = 0; k < 3; ++k) {
                result.m[i][j] += m[i][k] * other.m[k][j];
            }
        }
    }
    return result;
}

Mat3d Mat3d::operator*(double scalar) const {
    Mat3d result;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            result.m[i][j] = m[i][j] * scalar;
    return result;
}

Mat3d Mat3d::operator/(double scalar) const {
    return *this * (1.0 / scalar);
}

// Операторы присваивания
Mat3d& Mat3d::operator=(const Mat3d& other) {
    if (this != &other) {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                m[i][j] = other.m[i][j];
    }
    return *this;
}

Mat3d& Mat3d::operator+=(const Mat3d& other) {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            m[i][j] += other.m[i][j];
    return *this;
}

Mat3d& Mat3d::operator-=(const Mat3d& other) {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            m[i][j] -= other.m[i][j];
    return *this;
}

Mat3d& Mat3d::operator*=(const Mat3d& other) {
    *this = *this * other;
    return *this;
}

Mat3d& Mat3d::operator*=(double scalar) {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            m[i][j] *= scalar;
    return *this;
}

Mat3d& Mat3d::operator/=(double scalar) {
    return *this *= (1.0 / scalar);
}

// Умножение на вектор
Vec3d Mat3d::operator*(const Vec3d& vec) const {
    return Vec3d(
        m[0][0] * vec.x + m[0][1] * vec.y + m[0][2] * vec.z,
        m[1][0] * vec.x + m[1][1] * vec.y + m[1][2] * vec.z,
        m[2][0] * vec.x + m[2][1] * vec.y + m[2][2] * vec.z
    );
}

// Транспонирование
Mat3d Mat3d::transpose() const {
    return Mat3d(
        m[0][0], m[1][0], m[2][0],
        m[0][1], m[1][1], m[2][1],
        m[0][2], m[1][2], m[2][2]
    );
}

// Определитель
double Mat3d::determinant() const {
    return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
         - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0])
         + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
}

// Обратная матрица
Mat3d Mat3d::inverse() const {
    double det = determinant();
    if (std::abs(det) < 1e-15) {
        // Вырожденная матрица
        return Zero();
    }

    double inv_det = 1.0 / det;

    return Mat3d(
        (m[1][1] * m[2][2] - m[1][2] * m[2][1]) * inv_det,
        (m[0][2] * m[2][1] - m[0][1] * m[2][2]) * inv_det,
        (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * inv_det,

        (m[1][2] * m[2][0] - m[1][0] * m[2][2]) * inv_det,
        (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * inv_det,
        (m[0][2] * m[1][0] - m[0][0] * m[1][2]) * inv_det,

        (m[1][0] * m[2][1] - m[1][1] * m[2][0]) * inv_det,
        (m[0][1] * m[2][0] - m[0][0] * m[2][1]) * inv_det,
        (m[0][0] * m[1][1] - m[0][1] * m[1][0]) * inv_det
    );
}

// След матрицы
double Mat3d::trace() const {
    return m[0][0] + m[1][1] + m[2][2];
}

// Норма Фробениуса
double Mat3d::norm() const {
    double sum = 0.0;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            sum += m[i][j] * m[i][j];
    return std::sqrt(sum);
}

// Спектральная норма (упрощенная версия)
double Mat3d::spectralNorm() const {
    // Для точного вычисления нужны собственные значения
    // Используем оценку через норму Фробениуса
    return norm(); // Это верхняя оценка
}

// Собственные значения и векторы (упрощенный метод для симметричных матриц)
bool Mat3d::eigen(Vec3d& eigenvalues, Mat3d& eigenvectors) const {
    // Упрощенная реализация для симметричных матриц
    if (!isSymmetric()) {
        return false; // Только для симметричных матриц
    }

    // Простой итерационный метод для небольших матриц
    Mat3d A = *this;
    eigenvectors = Mat3d::Identity();

    const int max_iterations = 50;
    const double tolerance = 1e-12;

    for (int iter = 0; iter < max_iterations; ++iter) {
        // Находим максимальный внедиагональный элемент
        int p = 0, q = 1;
        double max_off_diag = std::abs(A(0,1));
        for (int i = 0; i < 3; ++i) {
            for (int j = i+1; j < 3; ++j) {
                if (std::abs(A(i,j)) > max_off_diag) {
                    max_off_diag = std::abs(A(i,j));
                    p = i;
                    q = j;
                }
            }
        }

        if (max_off_diag < tolerance) {
            break;
        }

        // Вычисляем вращение Якоби
        double app = A(p,p);
        double aqq = A(q,q);
        double apq = A(p,q);

        double tau = (aqq - app) / (2.0 * apq);
        double t = (tau >= 0 ? 1.0 : -1.0) / (std::abs(tau) + std::sqrt(1.0 + tau*tau));
        double c = 1.0 / std::sqrt(1.0 + t*t);
        double s = t * c;

        // Применяем вращение к A
        Mat3d J = Mat3d::Identity();
        J(p,p) = c; J(p,q) = s;
        J(q,p) = -s; J(q,q) = c;

        A = J.transpose() * A * J;
        eigenvectors = eigenvectors * J;
    }

    eigenvalues = Vec3d(A(0,0), A(1,1), A(2,2));
    return true;
}

// Симметричная часть
Mat3d Mat3d::symmetricPart() const {
    return (*this + this->transpose()) * 0.5;
}

// Антисимметричная часть
Mat3d Mat3d::antisymmetricPart() const {
    return (*this - this->transpose()) * 0.5;
}

// Проверки
bool Mat3d::isSymmetric(double tolerance) const {
    for (int i = 0; i < 3; ++i) {
        for (int j = i+1; j < 3; ++j) {
            if (std::abs(m[i][j] - m[j][i]) > tolerance) {
                return false;
            }
        }
    }
    return true;
}

bool Mat3d::isZero(double tolerance) const {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (std::abs(m[i][j]) > tolerance)
                return false;
    return true;
}

bool Mat3d::isIdentity(double tolerance) const {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            double expected = (i == j) ? 1.0 : 0.0;
            if (std::abs(m[i][j] - expected) > tolerance) {
                return false;
            }
        }
    }
    return true;
}

// Вывод
std::ostream& operator<<(std::ostream& os, const Mat3d& mat) {
    os << "[[" << mat.m[0][0] << ", " << mat.m[0][1] << ", " << mat.m[0][2] << "],\n"
       << " [" << mat.m[1][0] << ", " << mat.m[1][1] << ", " << mat.m[1][2] << "],\n"
       << " [" << mat.m[2][0] << ", " << mat.m[2][1] << ", " << mat.m[2][2] << "]]";
    return os;
}

// Внешние операторы
Mat3d operator*(double scalar, const Mat3d& mat) {
    return mat * scalar;
}

Vec3d operator*(const Vec3d& vec, const Mat3d& mat) {
    // Умножение вектора-строки на матрицу
    return Vec3d(
        vec.x * mat(0,0) + vec.y * mat(1,0) + vec.z * mat(2,0),
        vec.x * mat(0,1) + vec.y * mat(1,1) + vec.z * mat(2,1),
        vec.x * mat(0,2) + vec.y * mat(1,2) + vec.z * mat(2,2)
    );
}