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

    std::vector<double> mu(time_size - 1);
    mu[0] = 0;
    mu[time_size - 1] = 0;

    std::vector<double> z(time_size - 1);
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