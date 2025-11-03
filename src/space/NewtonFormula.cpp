//
// Created by magofrays on 11/3/25.
//

#include "NewtonFormula.h"
#include <cmath>
#include "../utils/IntegratingMethods3d.h"
#include <utility>

BodyState NewtonFormula::next_step(const BodyState& current_state) const
{
    Vec3d acceleration;
    for (const auto force_body : force_bodies )
    {
        BodyState force_body_state = force_body->get_body_state(current_state.time); // исправить
        Vec3d r_vec = force_body_state.position - current_state.position;
        const SpiceDouble r_cubed = std::pow(r_vec.norm(), 3);
        acceleration -= force_body->get_gravitational_parameter()*(r_vec)/r_cubed;
    }
    const Vec3d velocity = current_state.velocity + IntegratingMethods3d::rk4_method(step, integrate, acceleration);
    const Vec3d position = current_state.position + IntegratingMethods3d::rk4_method(step, integrate, velocity);
    const SpiceDouble time = current_state.time + step;
    return BodyState(position, velocity, time);

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
    set_object_name(object_name);
}

BodyState NewtonFormula::get_body_state(const SpiceDouble tdb)
{
    auto exact = body_states.find(tdb);
    if (exact != body_states.end()) {
        return exact->second;
    }

    auto upper = body_states.upper_bound(tdb);
    if (upper == body_states.end()) {
        calculate_to_target(
            body_states.lower_bound(tdb)->second,
            tdb
            );
        exact = body_states.find(tdb);
        if (exact != body_states.end()) {
            return exact->second;
        }
        upper = body_states.upper_bound(tdb);
    }
    return interpolate(
        std::prev(upper)->second,
        upper->second, tdb
        );
}

