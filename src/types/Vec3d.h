// Vec3d.h
#ifndef MESSENGERORBIT_VEC3D_H
#define MESSENGERORBIT_VEC3D_H

#include <cmath>
#include <iosfwd>

struct Vec3d
{
    long double x, y, z;
    Vec3d();
    Vec3d(long double x,  long double y, long double z);

    [[nodiscard]] long double norm () const;
    [[nodiscard]] long double squaredNorm() const;
    [[nodiscard]] Vec3d normalized() const;
    [[nodiscard]] Vec3d absDiff(const Vec3d& other) const;
    void normalize();

    // Скалярное произведение
    [[nodiscard]] long double dot(const Vec3d& other) const;

    // Векторное произведение
    [[nodiscard]] Vec3d cross(const Vec3d& other) const;

    // Расстояние до другой точки
    [[nodiscard]] long double distanceTo(const Vec3d& other) const;

    // Операторы присваивания
    Vec3d& operator+=(const Vec3d& other);
    Vec3d& operator-=(const Vec3d& other);
    Vec3d& operator*=(long double scalar);
    Vec3d& operator/=(long double scalar);

    // Унарные операторы
    Vec3d operator-() const;
};

// Бинарные операторы
Vec3d operator+(const Vec3d& a, const Vec3d& b);
Vec3d operator-(const Vec3d& a, const Vec3d& b);
Vec3d operator*(const Vec3d& a, long double scalar);
Vec3d operator*(long double scalar, const Vec3d& a);
Vec3d operator/(const Vec3d& a, long double scalar);

// Скалярное произведение
long double dot(const Vec3d& a, const Vec3d& b);

// Векторное произведение
Vec3d cross(const Vec3d& a, const Vec3d& b);

// Вывод
std::ostream& operator<<(std::ostream& os, const Vec3d& vec);

#endif //MESSENGERORBIT_VEC3D_H