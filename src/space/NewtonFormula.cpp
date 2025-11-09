//
// Created by magofrays on 11/3/25.
//

#include "NewtonFormula.h"
#include <cmath>
#include <iostream>

#include "../utils/IntegratingMethods3d.h"
#include <utility>

BodyState NewtonFormula::next_step(const BodyState& current_state) const {
    Vec3d k1_pos, k1_vel;
    Vec3d k2_pos, k2_vel;
    Vec3d k3_pos, k3_vel;
    Vec3d k4_pos, k4_vel;

    // k1
    k1_vel = calculate_acceleration(current_state.time, current_state.position);
    k1_pos = current_state.velocity;  // dx/dt = v

    // k2
    Vec3d pos_k2 = current_state.position + k1_pos * (step * 0.5);
    Vec3d vel_k2 = current_state.velocity + k1_vel * (step * 0.5);
    k2_vel = calculate_acceleration(current_state.time + step * 0.5, pos_k2);
    k2_pos = vel_k2;  // dx/dt = v (текущая скорость на подэтапе)

    // k3
    Vec3d pos_k3 = current_state.position + k2_pos * (step * 0.5);
    Vec3d vel_k3 = current_state.velocity + k2_vel * (step * 0.5);
    k3_vel = calculate_acceleration(current_state.time + step * 0.5, pos_k3);
    k3_pos = vel_k3;

    // k4
    Vec3d pos_k4 = current_state.position + k3_pos * step;
    Vec3d vel_k4 = current_state.velocity + k3_vel * step;
    k4_vel = calculate_acceleration(current_state.time + step, pos_k4);
    k4_pos = vel_k4;

    // Итоговое положение и скорость
    Vec3d new_position = current_state.position +
        (k1_pos + 2.0*k2_pos + 2.0*k3_pos + k4_pos) * (step / 6.0);
    Vec3d new_velocity = current_state.velocity +
        (k1_vel + 2.0*k2_vel + 2.0*k3_vel + k4_vel) * (step / 6.0);

    return BodyState(new_position, new_velocity, current_state.time + step);
}

Vec3d NewtonFormula::calculate_acceleration(SpiceDouble time, const Vec3d& current_position) const {
    Vec3d acceleration;
    for (SpaceObject* force_body : force_bodies) {
        BodyState force_body_state = force_body->get_body_state(time);
        Vec3d r_vec = current_position - force_body_state.position; // Вектор ОТ источника К телу
        double r = r_vec.norm();

        if (r > 1e-10) {
            double r_cubed = r * r * r;
            double mu = force_body->get_gravitational_parameter();
            acceleration -= mu * r_vec / r_cubed; // Ускорение направлено к источнику
        }
    }
    return acceleration;
}

Vec3d NewtonFormula::integrate(const Vec3d& derivative, const SpiceDouble step)
{
    return derivative * step;
}

BodyState NewtonFormula::calculate_to_target(BodyState current_state, SpiceDouble target_time)
{
    while (current_state.time < target_time )
    {
        current_state = next_step(current_state);
        add_body_state(current_state);
    }

    return current_state;
}


BodyState NewtonFormula::interpolate(const BodyState& first, const BodyState& second, const SpiceDouble current_time) {
    const double factor = (current_time - first.time) / (second.time - first.time);
    BodyState result;
    result.time = current_time;
    result.position = first.position + factor * (second.position - first.position);
    result.velocity = first.velocity + factor * (second.velocity - first.velocity);
    return result;
}


NewtonFormula::NewtonFormula(
    std::vector<SpaceObject*> force_bodies,
    const std::string & object_name,
    const BodyState & start_state,
    const SpiceDouble step
    ) : force_bodies(std::move(force_bodies)), step(step)
{
    add_body_state(start_state);
    this->start_state = start_state;
    Renderable::set_current_body_state(start_state);
    SpaceObject::set_object_name(object_name);
}

void NewtonFormula::add_force_body(SpaceObject* force_body)
{
    force_bodies.push_back(force_body);
}

void NewtonFormula::set_start_state(const BodyState& start_state)
{
    this->start_state = start_state;
    Renderable::set_current_body_state(start_state);
    body_states.clear();
    add_body_state(start_state);
}

BodyState NewtonFormula::get_body_state(const SpiceDouble tdb)
{
    auto current = get_current_body_state();
    if (tdb == current.time)
    {
        return current;
    }
    auto exact = body_states.find(tdb);
    if (exact != body_states.end()) {
        set_current_body_state(exact->second);
        return exact->second;
    }

    auto upper = body_states.upper_bound(tdb);
    if (upper == body_states.end()) {
        if (body_states.empty()) {
            throw std::runtime_error("No body states available");
        }
        auto last = body_states.rbegin();
        calculate_to_target(last->second, tdb); // fix for caching

        exact = body_states.find(tdb);
        if (exact != body_states.end()) {
            set_current_body_state(exact->second);
            return exact->second;
        }
        upper = body_states.upper_bound(tdb);
    }
    BodyState body = interpolate(
        std::prev(upper)->second,
        upper->second, tdb
        );
    this->current_body_state = body;
    return body;
}

