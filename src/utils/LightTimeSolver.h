//
// Created by sun on 05.12.2025.
//

#ifndef MESSENGERORBIT_LIGHTTIMESOLVER_H
#define MESSENGERORBIT_LIGHTTIMESOLVER_H

#include <map>
#include <SpiceZdf.h>
#include <string>

#include "Spline.h"

class LightTimeSolver {

    Spline* spline;
    double light_speed = 299792.458; // km/s

public:

    LightTimeSolver(std::map<SpiceDouble, BodyState>& points);
    SpiceDouble light_time_solve(SpiceDouble t3_tdb, std::string& dsn_id);
};


#endif //MESSENGERORBIT_LIGHTTIMESOLVER_H