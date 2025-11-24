//
// Created by magofrays on 10/25/25.
//

#include "Vec3d.h"
#include <cmath>

Vec3d::Vec3d() : x(0), y(0), z(0) {}

Vec3d::Vec3d(double x, double y, double z) : x(x), y(y), z(z) {}

double Vec3d::norm() const {
    return std::sqrt(x*x + y*y + z*z);
}

// Операторы-члены класса
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

// Внешние операторы
Vec3d operator+(const Vec3d& a, const Vec3d& b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

Vec3d operator-(const Vec3d& a, const Vec3d& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

Vec3d operator*(const Vec3d& a, double scalar) {
    return {a.x * scalar, a.y * scalar, a.z * scalar};
}

Vec3d operator*(double scalar, const Vec3d& a) {
    return {a.x * scalar, a.y * scalar, a.z * scalar};
}

Vec3d operator/(const Vec3d& a, double scalar) {
    return {a.x / scalar, a.y / scalar, a.z / scalar};
}

