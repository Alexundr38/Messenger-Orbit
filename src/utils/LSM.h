//
// Created by sun on 07.12.2025.
//

#ifndef MESSENGERORBIT_LSM_H
#define MESSENGERORBIT_LSM_H

#include <vector>
#include <cmath>

#include "DoplerComputer.h"
#include "../types/StateVector.h"

class LSM {

    StateVector state;
    Eigen::MatrixXd covariance;
    DopplerComputer* doppler_computer;

public:
    LSM(const StateVector& initial_guess, std::map<SpiceDouble, BodyState> &points);

    StateVector do_LSM(int max_iterations = 10, double convergence_tol = 1e-6);

    Eigen::VectorXd compute_partial_derivatives(SpiceDouble& full_time, SpiceDouble& d_full_time,
                                                SpiceDouble& middle_time, SpiceDouble& t0);
};


#endif //MESSENGERORBIT_LSM_H