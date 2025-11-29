//
// Created by magofrays on 11/3/25.
//

#include "NewtonFormula.h"
#include <cmath>
#include <iostream>

#include "../utils/IntegratingMethods3d.h"
#include <utility>

BodyState NewtonFormula::next_step(const BodyState& current_state) const {
        const double h = step;
        const Vec3d& y0_pos = current_state.position;
        const Vec3d& y0_vel = current_state.velocity;
        const double t0 = current_state.time;

        std::vector<Vec3d> k_pos(13), k_vel(13);

        // k1
        k_vel[0] = calculate_acceleration(t0, y0_pos);
        k_pos[0] = y0_vel;

        // k2
        Vec3d pos2 = y0_pos + k_pos[0] * (h * b21);
        Vec3d vel2 = y0_vel + k_vel[0] * (h * b21);
        k_vel[1] = calculate_acceleration(t0 + h * a2, pos2);
        k_pos[1] = vel2;

        // k3
        Vec3d pos3 = y0_pos + k_pos[0] * (h * b31) + k_pos[1] * (h * b32);
        Vec3d vel3 = y0_vel + k_vel[0] * (h * b31) + k_vel[1] * (h * b32);
        k_vel[2] = calculate_acceleration(t0 + h * a3, pos3);
        k_pos[2] = vel3;

        // k4
        Vec3d pos4 = y0_pos + k_pos[0] * (h * b41) + k_pos[2] * (h * b43);
        Vec3d vel4 = y0_vel + k_vel[0] * (h * b41) + k_vel[2] * (h * b43);
        k_vel[3] = calculate_acceleration(t0 + h * a4, pos4);
        k_pos[3] = vel4;

        // k5
        Vec3d pos5 = y0_pos + k_pos[0] * (h * b51) + k_pos[2] * (h * b53) + k_pos[3] * (h * b54);
        Vec3d vel5 = y0_vel + k_vel[0] * (h * b51) + k_vel[2] * (h * b53) + k_vel[3] * (h * b54);
        k_vel[4] = calculate_acceleration(t0 + h * a5, pos5);
        k_pos[4] = vel5;

        // k6
        Vec3d pos6 = y0_pos + k_pos[0] * (h * b61) + k_pos[3] * (h * b64) + k_pos[4] * (h * b65);
        Vec3d vel6 = y0_vel + k_vel[0] * (h * b61) + k_vel[3] * (h * b64) + k_vel[4] * (h * b65);
        k_vel[5] = calculate_acceleration(t0 + h * a6, pos6);
        k_pos[5] = vel6;

        // k7
        Vec3d pos7 = y0_pos + k_pos[0] * (h * b71) + k_pos[3] * (h * b74) + k_pos[4] * (h * b75) + k_pos[5] * (h * b76);
        Vec3d vel7 = y0_vel + k_vel[0] * (h * b71) + k_vel[3] * (h * b74) + k_vel[4] * (h * b75) + k_vel[5] * (h * b76);
        k_vel[6] = calculate_acceleration(t0 + h * a7, pos7);
        k_pos[6] = vel7;

        // k8
        Vec3d pos8 = y0_pos + k_pos[0] * (h * b81) + k_pos[3] * (h * b84) + k_pos[4] * (h * b85) +
                    k_pos[5] * (h * b86) + k_pos[6] * (h * b87);
        Vec3d vel8 = y0_vel + k_vel[0] * (h * b81) + k_vel[3] * (h * b84) + k_vel[4] * (h * b85) +
                    k_vel[5] * (h * b86) + k_vel[6] * (h * b87);
        k_vel[7] = calculate_acceleration(t0 + h * a8, pos8);
        k_pos[7] = vel8;

        // k9
        Vec3d pos9 = y0_pos + k_pos[0] * (h * b91) + k_pos[3] * (h * b94) + k_pos[4] * (h * b95) +
                    k_pos[5] * (h * b96) + k_pos[6] * (h * b97) + k_pos[7] * (h * b98);
        Vec3d vel9 = y0_vel + k_vel[0] * (h * b91) + k_vel[3] * (h * b94) + k_vel[4] * (h * b95) +
                    k_vel[5] * (h * b96) + k_vel[6] * (h * b97) + k_vel[7] * (h * b98);
        k_vel[8] = calculate_acceleration(t0 + h * a9, pos9);
        k_pos[8] = vel9;

        // k10
        Vec3d pos10 = y0_pos + k_pos[0] * (h * b101) + k_pos[3] * (h * b104) + k_pos[4] * (h * b105) +
                     k_pos[5] * (h * b106) + k_pos[6] * (h * b107) + k_pos[7] * (h * b108) + k_pos[8] * (h * b109);
        Vec3d vel10 = y0_vel + k_vel[0] * (h * b101) + k_vel[3] * (h * b104) + k_vel[4] * (h * b105) +
                     k_vel[5] * (h * b106) + k_vel[6] * (h * b107) + k_vel[7] * (h * b108) + k_vel[8] * (h * b109);
        k_vel[9] = calculate_acceleration(t0 + h * a10, pos10);
        k_pos[9] = vel10;

        // k11
        Vec3d pos11 = y0_pos + k_pos[0] * (h * b111) + k_pos[3] * (h * b114) + k_pos[4] * (h * b115) +
                     k_pos[5] * (h * b116) + k_pos[6] * (h * b117) + k_pos[7] * (h * b118) + k_pos[8] * (h * b119) +
                     k_pos[9] * (h * b1110);
        Vec3d vel11 = y0_vel + k_vel[0] * (h * b111) + k_vel[3] * (h * b114) + k_vel[4] * (h * b115) +
                     k_vel[5] * (h * b116) + k_vel[6] * (h * b117) + k_vel[7] * (h * b118) + k_vel[8] * (h * b119) +
                     k_vel[9] * (h * b1110);
        k_vel[10] = calculate_acceleration(t0 + h * a11, pos11);
        k_pos[10] = vel11;

        // k12
        Vec3d pos12 = y0_pos + k_pos[0] * (h * b121) + k_pos[3] * (h * b124) + k_pos[4] * (h * b125) +
                     k_pos[5] * (h * b126) + k_pos[6] * (h * b127) + k_pos[7] * (h * b128) + k_pos[8] * (h * b129) +
                     k_pos[9] * (h * b1210) + k_pos[10] * (h * b1211);
        Vec3d vel12 = y0_vel + k_vel[0] * (h * b121) + k_vel[3] * (h * b124) + k_vel[4] * (h * b125) +
                     k_vel[5] * (h * b126) + k_vel[6] * (h * b127) + k_vel[7] * (h * b128) + k_vel[8] * (h * b129) +
                     k_vel[9] * (h * b1210) + k_vel[10] * (h * b1211);
        k_vel[11] = calculate_acceleration(t0 + h * a12, pos12);
        k_pos[11] = vel12;

        // k13
        Vec3d pos13 = y0_pos + k_pos[0] * (h * b131) + k_pos[3] * (h * b134) + k_pos[4] * (h * b135) +
                     k_pos[5] * (h * b136) + k_pos[6] * (h * b137) + k_pos[7] * (h * b138) + k_pos[8] * (h * b139) +
                     k_pos[9] * (h * b1310) + k_pos[10] * (h * b1311);
        Vec3d vel13 = y0_vel + k_vel[0] * (h * b131) + k_vel[3] * (h * b134) + k_vel[4] * (h * b135) +
                     k_vel[5] * (h * b136) + k_vel[6] * (h * b137) + k_vel[7] * (h * b138) + k_vel[8] * (h * b139) +
                     k_vel[9] * (h * b1310) + k_vel[10] * (h * b1311);
        k_vel[12] = calculate_acceleration(t0 + h * a13, pos13);
        k_pos[12] = vel13;

        // Комбинируем с весами 8-го порядка
        Vec3d new_pos = y0_pos + h * (
            k_pos[0] * c1 + k_pos[5] * c6 + k_pos[6] * c7 + k_pos[7] * c8 +
            k_pos[8] * c9 + k_pos[9] * c10 + k_pos[10] * c11 + k_pos[11] * c12 + k_pos[12] * c13
        );

        Vec3d new_vel = y0_vel + h * (
            k_vel[0] * c1 + k_vel[5] * c6 + k_vel[6] * c7 + k_vel[7] * c8 +
            k_vel[8] * c9 + k_vel[9] * c10 + k_vel[10] * c11 + k_vel[11] * c12 + k_vel[12] * c13
        );

        return BodyState(new_pos, new_vel, t0 + h);
}

Vec3d NewtonFormula::calculate_acceleration(SpiceDouble time, const Vec3d& current_position) const {
    Vec3d acceleration;
    for (SpaceObject* body : force_bodies) {
        BodyState body_state = body->get_body_state(time);
        Vec3d r_vec = body_state.position - current_position;
        double r = r_vec.norm();

        if (r > 1e-10) {
            double mu = body->get_gravitational_parameter();
            double r3 = r * r * r;
            acceleration += mu * r_vec / r3;
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

