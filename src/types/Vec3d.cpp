// Vec3d.cpp
#include "Vec3d.h"
#include <cmath>
#include <iostream>

Vec3d::Vec3d() : x(0), y(0), z(0) {}

Vec3d::Vec3d(double x, double y, double z) : x(x), y(y), z(z) {}

double Vec3d::norm() const {
    return std::sqrt(x*x + y*y + z*z);
}

double Vec3d::squaredNorm() const {
    return x*x + y*y + z*z;
}

Vec3d Vec3d::normalized() const {
    double n = norm();
    if (n > 0) {
        return Vec3d(x / n, y / n, z / n);
    }
    return *this;
}

void Vec3d::normalize() {
    double n = norm();
    if (n > 0) {
        x /= n;
        y /= n;
        z /= n;
    }
}

double Vec3d::dot(const Vec3d& other) const {
    return x * other.x + y * other.y + z * other.z;
}

Vec3d Vec3d::cross(const Vec3d& other) const {
    return Vec3d(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    );
}

Vec3d& Vec3d::operator+=(const Vec3d& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vec3d& Vec3d::operator-=(const Vec3d& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Vec3d& Vec3d::operator*=(double scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Vec3d& Vec3d::operator/=(double scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}

Vec3d Vec3d::operator-() const {
    return Vec3d(-x, -y, -z);
}

// Внешние операторы
Vec3d operator+(const Vec3d& a, const Vec3d& b) {
    return Vec3d(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vec3d operator-(const Vec3d& a, const Vec3d& b) {
    return Vec3d(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vec3d operator*(const Vec3d& a, double scalar) {
    return Vec3d(a.x * scalar, a.y * scalar, a.z * scalar);
}

Vec3d operator*(double scalar, const Vec3d& a) {
    return Vec3d(a.x * scalar, a.y * scalar, a.z * scalar);
}

Vec3d operator/(const Vec3d& a, double scalar) {
    return Vec3d(a.x / scalar, a.y / scalar, a.z / scalar);
}

// Внешние функции
double dot(const Vec3d& a, const Vec3d& b) {
    return a.dot(b);
}

Vec3d cross(const Vec3d& a, const Vec3d& b) {
    return a.cross(b);
}

std::ostream& operator<<(std::ostream& os, const Vec3d& vec) {
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return os;
}