//
// Created by sun on 07.12.2025.
//

#ifndef MESSENGERORBIT_LSM_H
#define MESSENGERORBIT_LSM_H

#include <vector>
#include <cmath>

#include "DoplerComputer.h"
#include "../types/StateVector.h"
#include "CSVReader.h"
#include "../types/ExtendedBodyState.h"

class LSM {

    StateVector state;
    Eigen::MatrixXd covariance;
    DopplerComputer* doppler_computer;
    std::vector<ObservationData> observation_data;

public:
    LSM(const StateVector& initial_guess, std::map<SpiceDouble, ExtendedBodyState> &points, std::string file_name);

    StateVector do_LSM(int max_iterations = 10, double convergence_tol = 1e-6);

    Eigen::VectorXd compute_partial_derivatives(SpiceDouble& full_time, ObservationData& obs_data1, ObservationData& obs_data2);
};


#endif //MESSENGERORBIT_LSM_H