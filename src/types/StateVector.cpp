//
// Created by sun on 07.12.2025.
//

#include "StateVector.h"

void StateVector::add_state(BodyState state) {
    this->state = state;
}

void StateVector::add_param(double delta_f0, double f1, double f2) {
    this->delta_f0 = delta_f0;
    this->f1 = f1;
    this->f2 = f2;
}

Eigen::VectorXd StateVector::toVector() const {
    Eigen::VectorXd v(NUM_PARAMS);
    v << state.position.x, state.position.y, state.position.z, state.velocity.x,
            state.velocity.y, state.velocity.z, delta_f0, f1, f2;
    return v;
}

void StateVector::update(const Eigen::VectorXd& dx) {
    state.position.x += dx(0);
    state.position.y += dx(1);
    state.position.z += dx(2);
    state.velocity.x += dx(3);
    state.velocity.y += dx(4);
    state.velocity.z += dx(5);
    delta_f0 += dx(6);
    f1 += dx(7);
    f2 += dx(8);
}