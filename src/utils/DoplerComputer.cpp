//
// Created by sun on 06.12.2025.
//

#include "DoplerComputer.h"

#include "SpiceUsr.h"
#include <SpiceZpr.h>

#include "CSVReader.h"

DopplerComputer::DopplerComputer(std::map<SpiceDouble, ExtendedBodyState> &points) {
    this->light_time = new LightTimeSolver(points);
    std::map<SpiceDouble, ExtendedBodyState> jpl_points = CSVReader::read_csv_time("times.txt");
    this->light_time1 = new LightTimeSolver(jpl_points);
}

long double DopplerComputer::compute_doppler(SpiceDouble& t_recv, double &ref_freq,
                std::string& dsn_id, SpiceDouble& Tc) {

    SpiceDouble t_recv_first = t_recv - Tc/2;
    SpiceDouble t_recv_second = t_recv + Tc/2;

    //SpiceDouble messenger_start_first = light_time->light_time_solve(t_recv_first, dsn_id);
    //SpiceDouble messenger_start_second = light_time->light_time_solve(t_recv_second, dsn_id);

    SpiceDouble messenger_start_first = light_time->light_time_solve(t_recv_first, dsn_id);
    SpiceDouble messenger_start_second = light_time->light_time_solve(t_recv_second, dsn_id);

    SpiceDouble first_time = t_recv_first - messenger_start_first;
    SpiceDouble second_time = t_recv_second - messenger_start_second;


    //fix time


    /*char utc_first[50];
    et2utc_c(messenger_start_first, "C", 6, 50, utc_first);
    char utc_second[50];
    et2utc_c(messenger_start_second, "C", 6, 50, utc_second);*/

    SpiceDouble tai_first = unitim_c(messenger_start_first, "TDB", "TAI");
    SpiceDouble tai_second = unitim_c(messenger_start_second, "TDB", "TAI");

    SpiceDouble delta = (messenger_start_second - tai_second) - (messenger_start_first - tai_first); //(TDB _ TAI)[t2e] - (TDB _ TAI)[t2s]

    //full_time = messenger_start_second - messenger_start_first;

    long double doppler_freq = C2 * ref_freq * (second_time - first_time + delta) / Tc;
    //
    // middle_time = (messenger_start_second + messenger_start_first) / 2;
    //
    // d_full_time = full_time - (second_time - first_time + delta);
    //
    // doppler_freq -= C2 * (delta_f_t0 + f_t1 * (middle_time - t0) + f_t2 * (middle_time - t0) * (middle_time - t0)
    //     + (f_t2 / 12) * d_full_time * d_full_time) * (d_full_time / full_time);

    return doppler_freq;
}

LightTimeSolver* DopplerComputer::get_light_time_solver()
{
    return this->light_time;
}

// Vec3d DopplerComputer::get_d_r_c(SpiceDouble& t_recv, std::string& dsn_id)
// {
//     return this->light_time->get_vec(t_recv, dsn_id);
// }

DopplerComputer::~DopplerComputer() {
    delete this->light_time;
}