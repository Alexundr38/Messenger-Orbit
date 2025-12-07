//
// Created by sun on 07.12.2025.
//

#ifndef MESSENGERORBIT_STATEVECTOR_H
#define MESSENGERORBIT_STATEVECTOR_H

#include "BodyState.h"
#include <Eigen/Dense>

const int NUM_PARAMS = 9;

class StateVector {
public:
    BodyState state;
    double delta_f0;
    double f1;
    double f2;

    StateVector();

    void add_state(BodyState state);
    void add_param(double delta_f0, double f1, double f2);

    Eigen::VectorXd toVector() const;

    void update(const Eigen::VectorXd& dx);
};


#endif //MESSENGERORBIT_STATEVECTOR_H