#include "NewtonFormula.h"
#include <cmath>
#include <iostream>
#include <utility>

// void NewtonFormula::set_use_relativistic_corrections(bool use_relativistic_corrections)
// {
//     this->use_relativistic_corrections = use_relativistic_corrections;
// }

// BodyState NewtonFormula::next_step_implicit_newton(const BodyState& current_state) const {
//     const double h = step;
//     const double t_next = current_state.time + h;
//     BodyState x = next_step(current_state);
//     x = BodyState(x.position, x.velocity, t_next);
//
//     const int max_iterations = 3;  // Меньше для RK4
//     const double tolerance = 1e-12;
//
//     for (int iter = 0; iter < max_iterations; ++iter) {
//         Vec3d accel_current = calculate_acceleration(current_state.time, current_state.position);
//         Vec3d accel_next = calculate_acceleration(t_next, x.position);
//
//         // Невязка метода трапеций
//         Vec3d F_pos = x.position - current_state.position -
//             (h/2.0) * (current_state.velocity + x.velocity);
//         Vec3d F_vel = x.velocity - current_state.velocity -
//             (h/2.0) * (accel_current + accel_next);
//
//
//         if (iter == 0) {
//             x.position -= F_pos;
//             x.velocity -= F_vel;
//         } else {
//             Mat3d J_accel = calculate_acceleration_jacobian(t_next, x.position);
//             Vec3d delta_pos = -F_pos + (h/2.0) * F_vel;
//             Vec3d delta_vel = -F_vel + (h/2.0) * J_accel * F_pos;
//             x.position += delta_pos;
//             x.velocity += delta_vel;
//         }
//
//         double norm_F = std::sqrt(F_pos.squaredNorm() + F_vel.squaredNorm());
//         if (norm_F < tolerance) {
//             break;
//         }
//     }
//
//     return BodyState(x.position, x.velocity, t_next);
// }

BodyState NewtonFormula::trapezoidal_corrector_newton(const BodyState& current_state,
                                                      const BodyState& predictor_state) const {
    const double h = step;  // в сутках
    const double t_next = current_state.time + h;

    BodyState corrected = predictor_state;

    Vec3d accel_current = calculate_acceleration(current_state.time, current_state.position);
    Vec3d accel_predicted = calculate_acceleration(t_next, corrected.position);

    Vec3d corrected_vel = current_state.velocity +
        (h / 2.0) * (accel_current + accel_predicted);

    Vec3d corrected_pos = current_state.position +
        (h / 2.0) * (current_state.velocity + corrected_vel);

    // if (use_newton_methods) {
    //     Vec3d accel_corrected = calculate_acceleration(t_next, corrected_pos);
    //     corrected_vel = current_state.velocity +
    //         (h / 2.0) * (accel_current + accel_corrected);
    //     corrected_pos = current_state.position +
    //         (h / 2.0) * (current_state.velocity + corrected_vel);
    // }

    return BodyState(corrected_pos, corrected_vel, t_next);
}


NewtonFormula::NewtonFormula(const std::string& object_name, const BodyState& start_state, SpiceDouble step)
{
    this->object_name = object_name;
    this->start_state = start_state;
    this->step = step;
    this->size = 100;
    SpaceObject::set_object_name(object_name);
    NewtonFormula::set_current_body_state(start_state);
    add_body_state(start_state);
    previous_states.push_back(start_state);
}

NewtonFormula::NewtonFormula(
    std::vector<SpaceObject*> force_bodies,
    const std::string & object_name,
    const BodyState & start_state,
    const SpiceDouble step
) : force_bodies(std::move(force_bodies)), step(step), start_state(start_state)
{
    this->size = 100;
    SpaceObject::set_object_name(object_name);
    add_body_state(start_state);
    previous_states.push_back(start_state);
}

void NewtonFormula::set_object_name(const std::string& object_name) {
    SpaceObject::set_object_name(object_name);
}

BodyState NewtonFormula::get_body_state(const SpiceDouble tdb) {
    if (body_states->empty())
    {
        throw std::invalid_argument("No BodyState in container, can't integrate");
    }
    auto equal = body_states->find(tdb);
    if (equal != body_states->end())
    {
        return equal->second;
    }
    auto upper = body_states->upper_bound(tdb);
    if (upper == body_states->end())
    {
        auto result = calculate_to_target(std::prev(upper)->second, tdb);
        add_history_point(result.position);
        NewtonFormula::set_current_body_state(result);
        return result;
    }
    auto lower = std::prev(upper);
    if (lower == body_states->begin())
    {
        throw std::invalid_argument("Start position needed for integrating current time is before all ");
    }
    if (upper->first - lower->first < step )
    {
        interpolate(lower->second, upper->second, tdb);
    }
    auto result = calculate_to_target(lower->second, tdb);
    add_history_point(result.position);
    NewtonFormula::set_current_body_state(result);
    return result;

}

BodyState NewtonFormula::next_step(const BodyState& current_state) const {
    const long double h = step;
    const Vec3d& y0_pos = current_state.position;
    const Vec3d& y0_vel = current_state.velocity;
    const long double t0 = current_state.time;
    Vec3d k1_pos, k1_vel, k2_pos, k2_vel, k3_pos, k3_vel, k4_pos, k4_vel;

    k1_vel = calculate_acceleration(t0, y0_pos);
    k1_pos = y0_vel;

    Vec3d pos2 = y0_pos + h * 0.5f * k1_pos;
    Vec3d vel2 = y0_vel + h * 0.5f * k1_vel;
    k2_vel = calculate_acceleration(t0 + h * 0.5, pos2);
    k2_pos = vel2;

    Vec3d pos3 = y0_pos + h * 0.5f * k2_pos;
    Vec3d vel3 = y0_vel + h * 0.5f * k2_vel;
    k3_vel = calculate_acceleration(t0 + h * 0.5, pos3);
    k3_pos = vel3;

    Vec3d pos4 = y0_pos + h * k3_pos;
    Vec3d vel4 = y0_vel + h * k3_vel;
    k4_vel = calculate_acceleration(t0 + h, pos4);
    k4_pos = vel4;

    Vec3d new_pos = y0_pos + (h / 6.0) * (k1_pos + 2.0*k2_pos + 2.0*k3_pos + k4_pos);
    Vec3d new_vel = y0_vel + (h / 6.0) * (k1_vel + 2.0*k2_vel + 2.0*k3_vel + k4_vel);

    BodyState predictor_state(new_pos, new_vel, t0 + h);

    return predictor_state;

    // auto pos = current_state.position;
    // auto vel = current_state.velocity;
    // Vec3d new_vel = vel + calculate_acceleration(current_state.time, pos)*step;
    // Vec3d new_pos = pos + new_vel*step;
    // return BodyState(new_pos, new_vel, current_state.time+step);
}





Vec3d NewtonFormula::calculate_acceleration(SpiceDouble time, const Vec3d& current_position) const {
    Vec3d acceleration;
    for (SpaceObject* body : force_bodies) {
        BodyState body_state = body->get_body_state(time * day);
        Vec3d r_vec = body_state.position - current_position;
        long double r = r_vec.norm();
        long double mu = body->get_gravitational_parameter();
        long double r3 = r * r * r;
        acceleration += mu * r_vec / r3;
    }
    return acceleration;
}



// SpaceObject * NewtonFormula::getMercury()
// {
//     if (!this->mercury)
//     {
//         for (SpaceObject * object : force_bodies)
//         {
//             if (object->get_object_name() == "MERCURY BARYCENTER")
//             {
//                 this->mercury = object;
//                 break;
//             }
//         }
//     }
//     return this->mercury;
// }


// Vec3d NewtonFormula::calculate_acceleration_eih(SpiceDouble time, const Vec3d& position) {
//     SpaceObject* merc = getMercury();
//     SpaceObject* messenger = this;
//     Vec3d merc_vel = merc->get_body_state(time).velocity;
//     Vec3d messenger_acc;
//
// }

BodyState NewtonFormula::calculate_to_target(BodyState current_state, SpiceDouble target_time) {
    while (current_state.time < target_time) {
        current_state = next_step(current_state);

        previous_states.push_back(current_state);
        if (previous_states.size() > 4) {
            previous_states.pop_front();
        }
    }
    if (current_state.time != target_time)
    {
        auto prevState = std::prev(body_states->rbegin())->second;
        return interpolate(prevState, current_state, target_time);
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

void NewtonFormula::set_current_body_state(const BodyState& body_state)
{
    SpaceObject::set_current_body_state(body_state);
    (*body_states)[body_state.time] = body_state;
}


void NewtonFormula::add_force_body(SpaceObject* force_body) {
    force_bodies.push_back(force_body);
}
