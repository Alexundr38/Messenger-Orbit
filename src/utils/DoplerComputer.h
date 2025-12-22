//
// Created by sun on 06.12.2025.
//

#ifndef MESSENGERORBIT_DOPLERCOMPUTER_H
#define MESSENGERORBIT_DOPLERCOMPUTER_H

#include <map>

#include "LightTimeSolver.h"
#include "../types/BodyState.h"

inline constexpr double C2 = 880.0 / 749.0;

class DopplerComputer {
private:
    LightTimeSolver* light_time;
    LightTimeSolver* light_time1;
    //какой то ридер из csv, чтобы получить данные

public:
    DopplerComputer(std::map<SpiceDouble, ExtendedBodyState> &points);

    long double compute_doppler(SpiceDouble& t_recv, double &ref_freq,
                std::string& dsn_id, SpiceDouble& Tc);

    LightTimeSolver* get_light_time_solver();

    ~DopplerComputer();
};


#endif //MESSENGERORBIT_DOPLERCOMPUTER_H