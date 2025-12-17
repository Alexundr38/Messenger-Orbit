//
// Created by sun on 05.12.2025.
//

#ifndef MESSENGERORBIT_SPLINE_H
#define MESSENGERORBIT_SPLINE_H

#include <map>
#include <SpiceZdf.h>
#include <vector>
#include "../types/Vec3d.h"
#include "../types/Mat3d.h"

class Spline {
private:
    struct SplineData {
        double a, b, c, d;
        double start_dot;
    };

    std::vector<SplineData> splines_x;
    std::vector<SplineData> splines_y;
    std::vector<SplineData> splines_z;

    std::vector<SplineData> splines_a11;
    std::vector<SplineData> splines_a12;
    std::vector<SplineData> splines_a13;
    std::vector<SplineData> splines_a21;
    std::vector<SplineData> splines_a22;
    std::vector<SplineData> splines_a23;
    std::vector<SplineData> splines_a31;
    std::vector<SplineData> splines_a32;
    std::vector<SplineData> splines_a33;

public:
    Spline() = default;
    void build_all_splines(std::vector<SpiceDouble>& times, std::vector<Vec3d>& points);
    void build_mat3d_splines(std::vector<SpiceDouble>& times, std::vector<Mat3d>& points);
    std::vector<SplineData> build_spline(std::vector<SpiceDouble>& times, std::vector<long double>& points);
    Vec3d interpolate(SpiceDouble time);
    Mat3d interpolate_mat3d(SpiceDouble time);
    ~Spline() {
        splines_z.clear();
        splines_y.clear();
        splines_x.clear();
    };
};

#endif //MESSENGERORBIT_SPLINE_H