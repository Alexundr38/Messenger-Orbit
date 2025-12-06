//
// Created by sun on 06.12.2025.
//

#ifndef MESSENGERORBIT_DOPLERCOMPUTER_H
#define MESSENGERORBIT_DOPLERCOMPUTER_H

#include <map>

#include "LightTimeSolver.h"
#include "../types/BodyState.h"

class DopplerComputer {
private:
    LightTimeSolver* light_time;
    double c = 880.0 / 749.0;
    long double ref_freq; //из ридера
    long double delta_f_t0;
    long double f_t1;
    long double f_t2;
    SpiceDouble t0; // хз откуда -_-
    //какой то ридер из csv, чтобы получить данные

public:
    DopplerComputer(std::map<SpiceDouble, BodyState> &points,
                    long double delta_f_t0, long double f_t1, long double f_t2);
    long double compute_doppler(SpiceDouble& t_recv_first, SpiceDouble& t_recv_second, std::string& dsn_id);
};


#endif //MESSENGERORBIT_DOPLERCOMPUTER_H