//
// Created by sun on 06.12.2025.
//

#include "DoplerComputer.h"

DopplerComputer::DopplerComputer(std::map<SpiceDouble, BodyState> &points,
                    long double delta_f_t0, long double f_t1, long double f_t2) {
    this->light_time = new LightTimeSolver(points);
    this->delta_f_t0 = delta_f_t0;
    this->f_t1 = f_t1;
    this->f_t2 = f_t2;
}

long double DopplerComputer::compute_doppler(SpiceDouble& t_recv_first, SpiceDouble& t_recv_second, std::string& dsn_id) {
    SpiceDouble messenger_start_first = light_time->light_time_solve(t_recv_first, dsn_id);
    SpiceDouble messenger_start_second = light_time->light_time_solve(t_recv_second, dsn_id);

    SpiceDouble first_time = t_recv_first - messenger_start_first;
    SpiceDouble second_time = t_recv_second - messenger_start_second;

    SpiceDouble delta = 0; //(TDB _ TAI)[t2l] - (TDB _ TAI)[t2s] ????

    SpiceDouble full_time = second_time - first_time;

    long double doppler_freq = c * ref_freq * (second_time - first_time + delta) / full_time;

    SpiceDouble middle_time = (messenger_start_second + messenger_start_first) / 2;

    SpiceDouble d_full_time = full_time - (second_time - first_time + delta);

    doppler_freq -= c * (delta_f_t0 + f_t1 * (middle_time - t0) + f_t2 * (middle_time - t0) * (middle_time - t0)
        + (f_t2 / 12) * d_full_time * d_full_time) * (d_full_time / full_time);

    return doppler_freq;
}