//
// Created by sun on 05.12.2025.
//

#ifndef MESSENGERORBIT_SPLINE_H
#define MESSENGERORBIT_SPLINE_H

#include <map>
#include <SpiceZdf.h>
#include <vector>
#include "../types/Vec3d.h"

class Spline {
private:
    struct SplineData {
        double a, b, c, d;
        double start_dot;
    };

    std::vector<SplineData> splines_x;
    std::vector<SplineData> splines_y;
    std::vector<SplineData> splines_z;

public:
    Spline() = default;
    void build_all_splines(std::vector<SpiceDouble>& times, std::vector<Vec3d>& points);
    std::vector<SplineData> build_spline(std::vector<SpiceDouble>& times, std::vector<long double>& points);
    Vec3d interpolate(SpiceDouble time);
    ~Spline() {
        splines_z.clear();
        splines_y.clear();
        splines_x.clear();
    };
};

#endif //MESSENGERORBIT_SPLINE_H