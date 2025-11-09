//
// Created by magofrays on 11/3/25.
//

#include "NewtonFormula.h"
#include <cmath>
#include <iostream>

#include "../utils/IntegratingMethods3d.h"
#include <utility>

BodyState NewtonFormula::next_step(const BodyState& current_state) const
{
    // Функция, возвращающая производные системы: [v, a]
    auto system_derivative = [this](const BodyState& state) -> BodyState {
        Vec3d acceleration(0, 0, 0);
        for (const auto& force_body : force_bodies) {
            BodyState force_body_state = force_body->get_body_state(state.time); // Используем время состояния
            Vec3d r_vec = force_body_state.position - state.position;
            double distance = r_vec.norm();

            // Защита от деления на ноль
            if (distance < 1e-9) continue;

            const double r_cubed = distance * distance * distance;
            acceleration += force_body->get_gravitational_parameter() * r_vec / r_cubed;
        }
        // Возвращаем производные: dr/dt = v, dv/dt = a
        return BodyState(state.velocity, acceleration, 1.0); // dt/dt = 1
    };

    // Интегрируем всю систему за один шаг
    BodyState increment = IntegratingMethods3d::rk4_system_method(
        step, system_derivative, current_state);

    // Обновляем состояние
    const Vec3d new_position = current_state.position + increment.position;
    const Vec3d new_velocity = current_state.velocity + increment.velocity;
    const SpiceDouble new_time = current_state.time + increment.time; // Учитываем временной шаг

    return BodyState(new_position, new_velocity, new_time);
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
    auto exact = body_states.find(tdb);
    if (exact != body_states.end()) {
        this->current_body_state = exact->second;
        return exact->second;
    }

    auto upper = body_states.upper_bound(tdb);
    if (upper == body_states.end()) {
        if (body_states.empty()) {
            throw std::runtime_error("No body states available");
        }
        auto last = body_states.rbegin();
        calculate_to_target(last->second, tdb);

        exact = body_states.find(tdb);
        if (exact != body_states.end()) {
            this->current_body_state = exact->second;
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

