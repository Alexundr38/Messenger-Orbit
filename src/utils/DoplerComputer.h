//
// Created by sun on 06.12.2025.
//

#ifndef MESSENGERORBIT_DOPLERCOMPUTER_H
#define MESSENGERORBIT_DOPLERCOMPUTER_H

#include <map>

#include "LightTimeSolver.h"
#include "../types/BodyState.h"

double C2 = 880.0 / 749.0;

class DopplerComputer {
private:
    LightTimeSolver* light_time;
    long double delta_f_t0;
    long double f_t1;
    long double f_t2;
    //какой то ридер из csv, чтобы получить данные

public:
    DopplerComputer(std::map<SpiceDouble, BodyState> &points,
                    long double delta_f_t0, long double f_t1, long double f_t2);

    long double compute_doppler(SpiceDouble& t_recv_first, SpiceDouble& t_recv_second, long double &ref_freq,
                std::string& dsn_id, SpiceDouble& full_time, SpiceDouble& d_full_time, SpiceDouble& middle_time, SpiceDouble& t0) ;
};


#endif //MESSENGERORBIT_DOPLERCOMPUTER_H