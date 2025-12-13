//
// Created by sun on 05.12.2025.
//

#include "LightTimeSolver.h"

#include "SpaceObjectManager.h"

LightTimeSolver::LightTimeSolver(std::map<SpiceDouble, BodyState> &points) {
    spline_position = new Spline();
    spline_velocity = new Spline();

    std::vector<SpiceDouble> times;
    std::vector<Vec3d> positions;
    std::vector<Vec3d> velocities;

    for (const auto& pair : points) {
        times.push_back(pair.first);
        positions.push_back(pair.second.position);
        velocities.push_back(pair.second.velocity);
    }

    spline_position->build_all_splines(times, positions);
    spline_velocity->build_all_splines(times, velocities);
}

SpiceDouble LightTimeSolver::light_time_solve(SpiceDouble& t3_tdb, std::string& dsn_id) {
    BodyState state = SpaceObjectManager::get_DSN_state_at_time(t3_tdb, dsn_id);
    Vec3d messenger_point = spline_position->interpolate(t3_tdb);
    SpiceDouble dt = (state.position.distanceTo(messenger_point)) / C;
    SpiceDouble t2_tdb = t3_tdb - dt;

    for (int i = 0; i < 5; i++) {
        t2_tdb = t3_tdb - dt;
        messenger_point = spline_position->interpolate(t2_tdb);
        dt = (state.position.distanceTo(messenger_point)) / C;
    }

    return t2_tdb;
}

Vec3d LightTimeSolver::get_vec_2_3(SpiceDouble& t3_tdb, std::string& dsn_id)
{
    SpiceDouble t_send = light_time_solve(t3_tdb, dsn_id);
    return this->spline_position->interpolate(t_send) - this->spline_position->interpolate(t3_tdb);
}

Vec3d LightTimeSolver::get_vec_r_C(SpiceDouble t3_tdb)
{
    return this->spline_velocity->interpolate(t3_tdb);
}