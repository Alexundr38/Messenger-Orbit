#include "NewtonFormula.h"
#include <cmath>
#include <iostream>
#include <utility>

#include "../types/ExtendedBodyState.h"


void NewtonFormula::set_use_implicit(bool implicit_newton)
{
    this->implicit_newton = implicit_newton;
}

ExtendedBodyState NewtonFormula::next_step_implicit_newton(const ExtendedBodyState& current_state) const {
    const double h = step;
    const double t_next = current_state.time + h;

    ExtendedBodyState x = next_step(current_state);

    const int max_iterations = 50;
    const double tolerance = 1e-12;
    Vec3d accel_current = calculate_acceleration(current_state.time, current_state.position);

    for (int iter = 0; iter < max_iterations; ++iter) {
        Vec3d accel_next = calculate_acceleration(t_next, x.position);
        Vec3d F_pos = x.position - current_state.position -
                      (h/2.0) * (current_state.velocity + x.velocity);
        Vec3d F_vel = x.velocity - current_state.velocity -
                      (h/2.0) * (accel_current + accel_next);
        double norm_F = std::sqrt(F_pos.squaredNorm() + F_vel.squaredNorm());
        if (norm_F < tolerance) {
            break;
        }
        Mat3d J_accel = calculate_jacobian(t_next, x.position);
        Mat3d A = Mat3d::Identity() - (h*h/4.0) * J_accel;
        Vec3d b = -F_pos - (h/2.0) * F_vel;
        Vec3d delta_pos = A.inverse() * b;
        Vec3d delta_vel = (2.0/h) * (delta_pos + F_pos);
        x.position += delta_pos;
        x.velocity += delta_vel;
    }
    return ExtendedBodyState(x);
}

ExtendedBodyState NewtonFormula::trapezoidal_corrector_newton(const ExtendedBodyState& current_state,
                                                      const ExtendedBodyState& predictor_state) const {
    const double h = step;
    const double t_next = current_state.time + h;

    BodyState corrected = predictor_state;

    Vec3d accel_current = calculate_acceleration(current_state.time, current_state.position);
    Vec3d accel_predicted = calculate_acceleration(t_next, corrected.position);

    Vec3d corrected_vel = current_state.velocity +
        (h / 2.0) * (accel_current + accel_predicted);

    Vec3d corrected_pos = current_state.position +
        (h / 2.0) * (current_state.velocity + corrected_vel);

    return ExtendedBodyState(corrected_pos, corrected_vel, t_next);
}



NewtonFormula::NewtonFormula(const std::string& object_name, const ExtendedBodyState& start_state, SpiceDouble step)
{
    this->object_name = object_name;
    this->start_state = start_state;
    this->step = step;
    this->size = 100;
    SpaceObject::set_object_name(object_name);
    set_current_body_state(start_state);
    add_body_state(start_state);
}

NewtonFormula::NewtonFormula(
    std::vector<SpaceObject*> force_bodies,
    const std::string & object_name,
    const ExtendedBodyState & start_state,
    const SpiceDouble step
) : force_bodies(std::move(force_bodies)), step(step), start_state(start_state)
{
    this->object_name = object_name;
    this->start_state = start_state;
    this->step = step;
    this->size = 100;
    SpaceObject::set_object_name(object_name);
    set_current_body_state(start_state);
    add_body_state(start_state);
}

void NewtonFormula::set_object_name(const std::string& object_name) {
    SpaceObject::set_object_name(object_name);
}

BodyState NewtonFormula::get_body_state(const SpiceDouble tdb) {
    if (body_states->empty())
    {
        throw std::invalid_argument("No BodyState in container, can't integrate");
    }
    auto upper = body_states->upper_bound(tdb);
    auto result = calculate_to_target(std::prev(upper)->second, tdb);
    // add_history_point(result.position);
    set_current_body_state(result);
    return result;
}

ExtendedBodyState NewtonFormula::next_step(const ExtendedBodyState& current_state) const {
    const double h = step;
    const Vec3d& y0_pos = current_state.position;
    const Vec3d& y0_vel = current_state.velocity;
    const double t0 = current_state.time;
    Vec3d k1_pos, k1_vel, k2_pos, k2_vel, k3_pos, k3_vel, k4_pos, k4_vel;

    k1_vel = calculate_acceleration(t0, y0_pos);
    k1_pos = y0_vel;

    Vec3d pos2 = y0_pos + h * 0.5 * k1_pos;
    Vec3d vel2 = y0_vel + h * 0.5 * k1_vel;
    k2_vel = calculate_acceleration(t0 + h * 0.5, pos2);
    k2_pos = vel2;

    Vec3d pos3 = y0_pos + h * 0.5 * k2_pos;
    Vec3d vel3 = y0_vel + h * 0.5 * k2_vel;
    k3_vel = calculate_acceleration(t0 + h * 0.5, pos3);
    k3_pos = vel3;

    Vec3d pos4 = y0_pos + h * k3_pos;
    Vec3d vel4 = y0_vel + h * k3_vel;
    k4_vel = calculate_acceleration(t0 + h, pos4);
    k4_pos = vel4;

    Vec3d new_pos = y0_pos + (h / 6.0) * (k1_pos + 2.0*k2_pos + 2.0*k3_pos + k4_pos);
    Vec3d new_vel = y0_vel + (h / 6.0) * (k1_vel + 2.0*k2_vel + 2.0*k3_vel + k4_vel);

    return ExtendedBodyState(new_pos,
        new_vel, t0 + h, current_state.jacobian * calculate_jacobian(t0, y0_pos));
}

Mat3d NewtonFormula::calculate_jacobian(SpiceDouble time, const Vec3d& position) const
{
    Mat3d jacobian;
    for (SpaceObject * body : force_bodies)
    {
        Vec3d body_pos = body->get_body_state(time*day).position;
        SpiceDouble gm = body->get_gravitational_parameter();
        Vec3d diff = body_pos - position;
        long double up11 = gm*(-2*diff.x*diff.x + diff.y*diff.y + diff.z*diff.z);
        long double up12 = -3*gm*diff.x*diff.y;
        long double up13 = -3*gm*diff.x*diff.z;
        long double up21 = up12;
        long double up22 = gm*(diff.x*diff.x -2*diff.y*diff.y + diff.z*diff.z);
        long double up23 = -3*gm*diff.y*diff.z;
        long double up31 = up13;
        long double up32 = up23;
        long double up33 = gm*(diff.x*diff.x + diff.y*diff.y -2*diff.z*diff.z);
        long double down = std::sqrt(std::pow(diff.norm(), 5));
        jacobian[0][0] += up11/down;
        jacobian[0][1] += up12/down;
        jacobian[0][2] += up13/down;
        jacobian[1][0] += up21/down;
        jacobian[1][1] += up22/down;
        jacobian[1][2] += up23/down;
        jacobian[2][0] += up31/down;
        jacobian[2][1] += up32/down;
        jacobian[2][2] += up33/down;
    }
    return jacobian;
}

Vec3d NewtonFormula::calculate_acceleration(SpiceDouble time, const Vec3d& current_position) const {
    Vec3d acceleration;
    for (SpaceObject* body : force_bodies) {
        BodyState body_state = body->get_body_state(time * day);
        Vec3d r_vec = body_state.position - current_position;
        long double r = r_vec.norm();
        long double mu = body->get_gravitational_parameter();
        long double r3 = std::pow(r, 3);
        acceleration += mu * r_vec / r3;
        std::string body_name = body->get_object_name();
        if (body_name == "MERCURY BARYCENTER") {
            constexpr double R_mercury_AU = 2439.7 / au;  // Точный радиус
            constexpr double J2 = 6.0e-5;

            if (r > R_mercury_AU) {
                Vec3d unit_vec = r_vec / r;
                double z_over_r = unit_vec.z;
                double j2_factor = 1.5 * J2 * (R_mercury_AU / r) * (R_mercury_AU / r) * (mu / (r * r));;

                Vec3d a_j2;
                a_j2.x = unit_vec.x * (5 * z_over_r * z_over_r - 1) * j2_factor;
                a_j2.y = unit_vec.y * (5 * z_over_r * z_over_r - 1) * j2_factor;
                a_j2.z = unit_vec.z * (5 * z_over_r * z_over_r - 3) * j2_factor;

                acceleration += a_j2;
            }
        }
    }
    return acceleration;
}

ExtendedBodyState NewtonFormula::calculate_to_target(ExtendedBodyState current_state, SpiceDouble target_time) {
    while (target_time - current_state.time > DP_TOL) {
        if (implicit_newton)
        {
            current_state = next_step_implicit_newton(current_state);
        }
        else
        {
            current_state = next_step(current_state);
        }
    }
    return current_state;
}

// Если ищем состояние среди найденных шагов
ExtendedBodyState NewtonFormula::interpolate(const ExtendedBodyState& first, const ExtendedBodyState& second, const SpiceDouble current_time) {
    const double factor = (current_time - first.time) / (second.time - first.time);
    ExtendedBodyState result;
    result.time = current_time;
    result.position = first.position + factor * (second.position - first.position);
    result.velocity = first.velocity + factor * (second.velocity - first.velocity);
    return result;
}

void NewtonFormula::set_current_body_state(const ExtendedBodyState& body_state)
{
    SpaceObject::set_current_body_state(body_state);
    (*body_states)[body_state.time] = body_state;
}

void NewtonFormula::add_force_body(SpaceObject* force_body) {
    force_bodies.push_back(force_body);
}
