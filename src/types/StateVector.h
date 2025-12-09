//
// Created by sun on 07.12.2025.
//

#ifndef MESSENGERORBIT_STATEVECTOR_H
#define MESSENGERORBIT_STATEVECTOR_H

#include "BodyState.h"
#include <Eigen/Dense>

inline constexpr int NUM_PARAMS = 3;

class StateVector {
public:
    BodyState state;

    StateVector() = default;

    void add_state(BodyState state);

    Eigen::VectorXd toVector() const;

    void update(const Eigen::VectorXd& dx);
};


#endif //MESSENGERORBIT_STATEVECTOR_H