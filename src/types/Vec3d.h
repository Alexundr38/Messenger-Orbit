// Vec3d.h
#ifndef MESSENGERORBIT_VEC3D_H
#define MESSENGERORBIT_VEC3D_H

#include <cmath>
#include <iosfwd>

struct Vec3d
{
    double x, y, z;
    Vec3d();
    Vec3d(double x,  double y, double z);

    [[nodiscard]] double norm () const;
    [[nodiscard]] double squaredNorm() const;
    [[nodiscard]] Vec3d normalized() const;
    void normalize();

    // Скалярное произведение
    [[nodiscard]] double dot(const Vec3d& other) const;

    // Векторное произведение
    [[nodiscard]] Vec3d cross(const Vec3d& other) const;

    // Операторы присваивания
    Vec3d& operator+=(const Vec3d& other);
    Vec3d& operator-=(const Vec3d& other);
    Vec3d& operator*=(double scalar);
    Vec3d& operator/=(double scalar);

    // Унарные операторы
    Vec3d operator-() const;
};

// Бинарные операторы
Vec3d operator+(const Vec3d& a, const Vec3d& b);
Vec3d operator-(const Vec3d& a, const Vec3d& b);
Vec3d operator*(const Vec3d& a, double scalar);
Vec3d operator*(double scalar, const Vec3d& a);
Vec3d operator/(const Vec3d& a, double scalar);

// Скалярное произведение
double dot(const Vec3d& a, const Vec3d& b);

// Векторное произведение
Vec3d cross(const Vec3d& a, const Vec3d& b);

// Вывод
std::ostream& operator<<(std::ostream& os, const Vec3d& vec);

#endif //MESSENGERORBIT_VEC3D_H