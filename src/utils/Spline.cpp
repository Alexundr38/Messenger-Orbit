//
// Created by sun on 05.12.2025.
//

#include "Spline.h"

#include <iostream>
#include <utility>

void Spline::build_all_splines(std::vector<SpiceDouble>& times, std::vector<Vec3d>& points){
    std::vector<long double> x;
    std::vector<long double> y;
    std::vector<long double> z;
    for (int i = 0; i < times.size(); i++) {
        x.push_back(points[i].x);
        y.push_back(points[i].y);
        z.push_back(points[i].z);
    }

    splines_x = build_spline(times, x);
    splines_y = build_spline(times, y);
    splines_z = build_spline(times, z);
}

void Spline::build_mat3d_splines(std::vector<SpiceDouble>& times, std::vector<Mat3d>& points) {
    std::vector<long double> a11, a12, a13, a21, a22, a23, a31, a32, a33;
    for (int i = 0; i < times.size(); i++) {
        a11.push_back(points[i].m[0][0]);
        a12.push_back(points[i].m[0][1]);
        a13.push_back(points[i].m[0][2]);
        a21.push_back(points[i].m[1][0]);
        a22.push_back(points[i].m[1][1]);
        a23.push_back(points[i].m[1][2]);
        a31.push_back(points[i].m[2][0]);
        a32.push_back(points[i].m[2][1]);
        a33.push_back(points[i].m[2][2]);
    }

    splines_a11 = build_spline(times, a11);
    splines_a12 = build_spline(times, a12);
    splines_a13 = build_spline(times, a13);
    splines_a21 = build_spline(times, a21);
    splines_a22 = build_spline(times, a22);
    splines_a23 = build_spline(times, a23);
    splines_a31 = build_spline(times, a31);
    splines_a32 = build_spline(times, a32);
    splines_a33 = build_spline(times, a33);
}


std::vector<Spline::SplineData> Spline::build_spline(std::vector<SpiceDouble>& times, std::vector<long double>& points) {

    int time_size = times.size();
    std::vector<SplineData> spline_data(time_size);

    for (int i = 0; i < time_size; i++) {
        spline_data[i].a = points[i];
        spline_data[i].start_dot = times[i];
    }

    std::vector<double> h(time_size - 1);
    for (int i = 0; i < time_size - 1; i++) {
        h[i] = times[i + 1] - times[i];
    }

    std::vector<double> alpha(time_size - 1);
    for (int i = 1; i < time_size - 1; i++) {
        alpha[i] = (3 / h[i]) * (spline_data[i+1].a - spline_data[i].a) -
                   (3 / h[i-1]) * (spline_data[i].a - spline_data[i-1].a);
    }

    std::vector<double> l(time_size);
    l[0] = 1;
    l[time_size - 1] = 1;

    std::vector<double> mu(time_size);
    mu[0] = 0;
    mu[time_size - 1] = 0;

    std::vector<double> z(time_size);
    z[0] = 0;
    z[time_size - 1] = 0;

    for (int i = 1; i < time_size - 1; i++) {
        l[i] = 2 * (times[i+1] - times[i-1]) - (h[i] * mu[i-1]);
        mu[i] = h[i] / l[i];
        z[i] = (alpha[i] - h[i-1] * z[i-1]) / l[i];
    }

    spline_data[time_size - 1].c = 0;

    for (int i = time_size - 2; i >= 0; i--) {
        spline_data[i].c = z[i] - mu[i] * spline_data[i+1].c;
        spline_data[i].b = (spline_data[i+1].a - spline_data[i].a) / h[i] -
                            - h[i] * (spline_data[i+1].c + 2 * spline_data[i].c) / 3;
        spline_data[i].d = (spline_data[i+1].c - spline_data[i].c) / (3 * h[i]);
    }

    spline_data.pop_back();

    return spline_data;
}


Vec3d Spline::interpolate(SpiceDouble time) {
    int i = 0;
    int spline_size = splines_x.size() - 1;

    if (time <= splines_x[0].start_dot) {
        i = 0;
    } else if (time >= splines_x[spline_size].start_dot) {
        i = spline_size;
    } else {
        for (int k = 0; k < spline_size; k++) {
            if (time <= splines_x[k].start_dot) {
                i = k;
                break;
            }
        }
    }


    double dt = time - splines_x[i].start_dot;
    return Vec3d(splines_x[i].a + splines_x[i].b * dt +
                 splines_x[i].c * dt * dt + splines_x[i].d * dt * dt * dt,
                 splines_y[i].a + splines_y[i].b * dt +
                 splines_y[i].c * dt * dt + splines_y[i].d * dt * dt * dt,
                 splines_z[i].a + splines_z[i].b * dt +
                 splines_z[i].c * dt * dt + splines_z[i].d * dt * dt * dt
           );
}

Mat3d Spline::interpolate_mat3d(SpiceDouble time) {
    int i = 0;
    int spline_size = splines_a11.size() - 1;

    if (time <= splines_a11[0].start_dot) {
        i = 0;
    } else if (time >= splines_a11[spline_size].start_dot) {
        i = spline_size;
    } else {
        for (int k = 0; k < spline_size; k++) {
            if (time <= splines_a11[k].start_dot) {
                i = k;
                break;
            }
        }
    }


    double dt = time - splines_a11[i].start_dot;
    return Mat3d(splines_a11[i].a + splines_a11[i].b * dt +
                 splines_a11[i].c * dt * dt + splines_a11[i].d * dt * dt * dt,
                 splines_a12[i].a + splines_a12[i].b * dt +
                 splines_a12[i].c * dt * dt + splines_a12[i].d * dt * dt * dt,
                 splines_a13[i].a + splines_a13[i].b * dt +
                 splines_a13[i].c * dt * dt + splines_a13[i].d * dt * dt * dt,
                 splines_a21[i].a + splines_a21[i].b * dt +
                 splines_a21[i].c * dt * dt + splines_a21[i].d * dt * dt * dt,
                 splines_a22[i].a + splines_a22[i].b * dt +
                 splines_a22[i].c * dt * dt + splines_a22[i].d * dt * dt * dt,
                 splines_a23[i].a + splines_a23[i].b * dt +
                 splines_a23[i].c * dt * dt + splines_a23[i].d * dt * dt * dt,
                 splines_a31[i].a + splines_a31[i].b * dt +
                 splines_a31[i].c * dt * dt + splines_a31[i].d * dt * dt * dt,
                 splines_a32[i].a + splines_a32[i].b * dt +
                 splines_a32[i].c * dt * dt + splines_a32[i].d * dt * dt * dt,
                 splines_a33[i].a + splines_a33[i].b * dt +
                 splines_a33[i].c * dt * dt + splines_a33[i].d * dt * dt * dt
           );
}