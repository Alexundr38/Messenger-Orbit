//
// Created by magofrays on 11/30/25.
//

#ifndef MESSENGERORBIT_MAT3D_H
#define MESSENGERORBIT_MAT3D_H

// Mat3d.h
#ifndef MAT3D_H
#define MAT3D_H

#include "Vec3d.h"
#include <cmath>
#include <iostream>

class Mat3d {
public:
    long double m[3][3];

    Mat3d();
    Mat3d(long double m00, long double m01, long double m02,
          long double m10, long double m11, long double m12,
          long double m20, long double m21, long double m22);
    Mat3d(const Mat3d& other);

    static Mat3d Zero();
    static Mat3d Identity();
    static Mat3d Diagonal(long double d0, long double d1, long double d2);

    long double& operator()(int i, int j);
    const long double& operator()(int i, int j) const;
    long double* operator[](int i);
    const long double* operator[](int i) const;

    Mat3d operator+(const Mat3d& other) const;
    Mat3d operator-(const Mat3d& other) const;
    Mat3d operator*(const Mat3d& other) const;
    Mat3d operator*(long double scalar) const;
    Mat3d operator/(long double scalar) const;

    Mat3d& operator=(const Mat3d& other);
    Mat3d& operator+=(const Mat3d& other);
    Mat3d& operator-=(const Mat3d& other);
    Mat3d& operator*=(const Mat3d& other);
    Mat3d& operator*=(long double scalar);
    Mat3d& operator/=(long double scalar);

    Vec3d operator*(const Vec3d& vec) const;

    [[nodiscard]] Mat3d transpose() const;

    [[nodiscard]] long double determinant() const;

    [[nodiscard]] Mat3d inverse() const;

    [[nodiscard]] long double trace() const;

    [[nodiscard]] long double norm() const;
    [[nodiscard]] long double spectralNorm() const;

    bool eigen(Vec3d& eigenvalues, Mat3d& eigenvectors) const;

    [[nodiscard]] Mat3d symmetricPart() const;

    [[nodiscard]] Mat3d antisymmetricPart() const;

    [[nodiscard]] bool isSymmetric(long double tolerance = 1e-12) const;
    [[nodiscard]] bool isZero(long double tolerance = 1e-12) const;
    [[nodiscard]] bool isIdentity(long double tolerance = 1e-12) const;

    friend std::ostream& operator<<(std::ostream& os, const Mat3d& mat);
};

Mat3d operator*(long double scalar, const Mat3d& mat);
Vec3d operator*(const Vec3d& vec, const Mat3d& mat);

#endif // MAT3D_H

#endif //MESSENGERORBIT_MAT3D_H