//
// Created by sun on 05.12.2025.
//

#ifndef MESSENGERORBIT_LIGHTTIMESOLVER_H
#define MESSENGERORBIT_LIGHTTIMESOLVER_H

#include <map>
#include <SpiceZdf.h>
#include <string>

#include "Constants.h"
#include "../types/ExtendedBodyState.h"

#include "Spline.h"

inline constexpr double C = (299792.458 / au) * day; // au/d

class LightTimeSolver {

    Spline* spline_position;
    Spline* spline_velocity;
    Spline* spline_jacobian;

public:

    LightTimeSolver(std::map<SpiceDouble, ExtendedBodyState>& points);
    SpiceDouble light_time_solve(SpiceDouble& t3_tdb, std::string& dsn_id);
    Vec3d get_vec_2_3(SpiceDouble& t3_tdb, std::string& dsn_id);
    Vec3d get_vec_r_C(SpiceDouble t3_tdb);
    Mat3d get_spline_mat3d(SpiceDouble time_tdb);
    ~LightTimeSolver();
};


#endif //MESSENGERORBIT_LIGHTTIMESOLVER_H