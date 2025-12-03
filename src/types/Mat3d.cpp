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

Mat3d::Mat3d(long double m00, long double m01, long double m02,
             long double m10, long double m11, long double m12,
             long double m20, long double m21, long double m22) {
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

Mat3d Mat3d::Diagonal(long double d0, long double d1, long double d2) {
    return Mat3d(d0, 0,  0,
                 0,  d1, 0,
                 0,  0,  d2);
}

// Операторы доступа
long double& Mat3d::operator()(int i, int j) {
    return m[i][j];
}

const long double& Mat3d::operator()(int i, int j) const {
    return m[i][j];
}

long double* Mat3d::operator[](int i) {
    return m[i];
}

const long double* Mat3d::operator[](int i) const {
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

Mat3d Mat3d::operator*(long double scalar) const {
    Mat3d result;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            result.m[i][j] = m[i][j] * scalar;
    return result;
}

Mat3d Mat3d::operator/(long double scalar) const {
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

Mat3d& Mat3d::operator*=(long double scalar) {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            m[i][j] *= scalar;
    return *this;
}

Mat3d& Mat3d::operator/=(long double scalar) {
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

long double Mat3d::determinant() const {
    return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
         - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0])
         + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
}

Mat3d Mat3d::inverse() const {
    long double det = determinant();
    if (std::abs(det) < 1e-15) {
        // Вырожденная матрица
        return Zero();
    }

    long double inv_det = 1.0 / det;

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

long double Mat3d::trace() const {
    return m[0][0] + m[1][1] + m[2][2];
}


long double Mat3d::norm() const {
    long double sum = 0.0;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            sum += m[i][j] * m[i][j];
    return std::sqrt(sum);
}

long double Mat3d::spectralNorm() const {

    return norm(); // Это верхняя оценка
}

bool Mat3d::eigen(Vec3d& eigenvalues, Mat3d& eigenvectors) const {

    if (!isSymmetric()) {
        return false;
    }

    Mat3d A = *this;
    eigenvectors = Mat3d::Identity();

    const int max_iterations = 50;
    const long double tolerance = 1e-12;

    for (int iter = 0; iter < max_iterations; ++iter) {
        int p = 0, q = 1;
        long double max_off_diag = std::abs(A(0,1));
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

        long double app = A(p,p);
        long double aqq = A(q,q);
        long double apq = A(p,q);

        long double tau = (aqq - app) / (2.0 * apq);
        long double t = (tau >= 0 ? 1.0 : -1.0) / (std::abs(tau) + std::sqrt(1.0 + tau*tau));
        long double c = 1.0 / std::sqrt(1.0 + t*t);
        long double s = t * c;

        Mat3d J = Mat3d::Identity();
        J(p,p) = c; J(p,q) = s;
        J(q,p) = -s; J(q,q) = c;

        A = J.transpose() * A * J;
        eigenvectors = eigenvectors * J;
    }

    eigenvalues = Vec3d(A(0,0), A(1,1), A(2,2));
    return true;
}

Mat3d Mat3d::symmetricPart() const {
    return (*this + this->transpose()) * 0.5;
}

Mat3d Mat3d::antisymmetricPart() const {
    return (*this - this->transpose()) * 0.5;
}

bool Mat3d::isSymmetric(long double tolerance) const {
    for (int i = 0; i < 3; ++i) {
        for (int j = i+1; j < 3; ++j) {
            if (std::abs(m[i][j] - m[j][i]) > tolerance) {
                return false;
            }
        }
    }
    return true;
}

bool Mat3d::isZero(long double tolerance) const {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (std::abs(m[i][j]) > tolerance)
                return false;
    return true;
}

bool Mat3d::isIdentity(long double tolerance) const {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            long double expected = (i == j) ? 1.0 : 0.0;
            if (std::abs(m[i][j] - expected) > tolerance) {
                return false;
            }
        }
    }
    return true;
}

std::ostream& operator<<(std::ostream& os, const Mat3d& mat) {
    os << "[[" << mat.m[0][0] << ", " << mat.m[0][1] << ", " << mat.m[0][2] << "],\n"
       << " [" << mat.m[1][0] << ", " << mat.m[1][1] << ", " << mat.m[1][2] << "],\n"
       << " [" << mat.m[2][0] << ", " << mat.m[2][1] << ", " << mat.m[2][2] << "]]";
    return os;
}

Mat3d operator*(long double scalar, const Mat3d& mat) {
    return mat * scalar;
}

Vec3d operator*(const Vec3d& vec, const Mat3d& mat) {
    return Vec3d(
        vec.x * mat(0,0) + vec.y * mat(1,0) + vec.z * mat(2,0),
        vec.x * mat(0,1) + vec.y * mat(1,1) + vec.z * mat(2,1),
        vec.x * mat(0,2) + vec.y * mat(1,2) + vec.z * mat(2,2)
    );
}