//
// Created by sun on 05.12.2025.
//

#ifndef MESSENGERORBIT_LIGHTTIMESOLVER_H
#define MESSENGERORBIT_LIGHTTIMESOLVER_H

#include <map>
#include <SpiceZdf.h>
#include <string>
#include "../types/BodyState.h"

#include "Spline.h"

inline constexpr double C = 299792.458; // km/s

class LightTimeSolver {

    Spline* spline_position;
    Spline* spline_velocity;

public:

    LightTimeSolver(std::map<SpiceDouble, BodyState>& points);
    SpiceDouble light_time_solve(SpiceDouble& t3_tdb, std::string& dsn_id);
    Vec3d get_vec_2_3(SpiceDouble& t3_tdb, std::string& dsn_id);
    Vec3d get_vec_r_C(SpiceDouble t3_tdb);
};


#endif //MESSENGERORBIT_LIGHTTIMESOLVER_H