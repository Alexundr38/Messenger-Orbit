//
// Created by sun on 07.12.2025.
//

#include "StateVector.h"

void StateVector::add_state(BodyState state) {
    this->state = state;
}


Eigen::VectorXd StateVector::toVector() const {
    Eigen::VectorXd v(NUM_PARAMS);
    v << state.position.x, state.position.y, state.position.z;
    return v;
}

void StateVector::update(const Eigen::VectorXd& dx) {
    state.position.x += dx(0);
    state.position.y += dx(1);
    state.position.z += dx(2);
}