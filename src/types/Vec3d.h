//
// Created by magofrays on 10/25/25.
//

#ifndef MESSENGERORBIT_VEC3D_H
#define MESSENGERORBIT_VEC3D_H


struct Vec3d
{
    double x, y, z;
    Vec3d();
    Vec3d(double x,  double y, double z);

    double norm () const;
    Vec3d& operator+=(const Vec3d& other);
    Vec3d& operator-=(const Vec3d& other);
    Vec3d& operator*=(double scalar);
    Vec3d& operator/=(double scalar);
};

Vec3d operator+(const Vec3d& a, const Vec3d& b);
Vec3d operator-(const Vec3d& a, const Vec3d& b);
Vec3d operator*(const Vec3d& a, double scalar);
Vec3d operator*(double scalar, const Vec3d& a);
Vec3d operator/(const Vec3d& a, double scalar);

#endif //MESSENGERORBIT_VEC3D_H