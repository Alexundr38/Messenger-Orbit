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
    double m[3][3];

    Mat3d();
    Mat3d(double m00, double m01, double m02,
          double m10, double m11, double m12,
          double m20, double m21, double m22);
    Mat3d(const Mat3d& other);

    static Mat3d Zero();
    static Mat3d Identity();
    static Mat3d Diagonal(double d0, double d1, double d2);

    double& operator()(int i, int j);
    const double& operator()(int i, int j) const;
    double* operator[](int i);
    const double* operator[](int i) const;

    Mat3d operator+(const Mat3d& other) const;
    Mat3d operator-(const Mat3d& other) const;
    Mat3d operator*(const Mat3d& other) const;
    Mat3d operator*(double scalar) const;
    Mat3d operator/(double scalar) const;

    Mat3d& operator=(const Mat3d& other);
    Mat3d& operator+=(const Mat3d& other);
    Mat3d& operator-=(const Mat3d& other);
    Mat3d& operator*=(const Mat3d& other);
    Mat3d& operator*=(double scalar);
    Mat3d& operator/=(double scalar);

    Vec3d operator*(const Vec3d& vec) const;

    Mat3d transpose() const;

    double determinant() const;

    Mat3d inverse() const;

    double trace() const;

    double norm() const;
    double spectralNorm() const;

    bool eigen(Vec3d& eigenvalues, Mat3d& eigenvectors) const;

    Mat3d symmetricPart() const;

    Mat3d antisymmetricPart() const;

    bool isSymmetric(double tolerance = 1e-12) const;
    bool isZero(double tolerance = 1e-12) const;
    bool isIdentity(double tolerance = 1e-12) const;

    friend std::ostream& operator<<(std::ostream& os, const Mat3d& mat);
};

Mat3d operator*(double scalar, const Mat3d& mat);
Vec3d operator*(const Vec3d& vec, const Mat3d& mat);

#endif // MAT3D_H

#endif //MESSENGERORBIT_MAT3D_H