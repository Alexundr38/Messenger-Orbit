//
// Created by sun on 05.12.2025.
//

#include "LightTimeSolver.h"

#include "SpaceObjectManager.h"

LightTimeSolver::LightTimeSolver(std::map<SpiceDouble, ExtendedBodyState> &points) {
    spline_position = new Spline();
    spline_velocity = new Spline();
    spline_jacobian = new Spline();

    std::vector<SpiceDouble> times;
    std::vector<Vec3d> positions;
    std::vector<Vec3d> velocities;
    std::vector<Mat3d> matrices;

    for (const auto& pair : points) {
        times.push_back(pair.first);
        positions.push_back(pair.second.position);
        velocities.push_back(pair.second.velocity);
        matrices.push_back(pair.second.jacobian);
    }

    spline_position->build_all_splines(times, positions);
    spline_velocity->build_all_splines(times, velocities);
    spline_jacobian->build_mat3d_splines(times, matrices);
}

SpiceDouble LightTimeSolver::light_time_solve(SpiceDouble& t3_tdb, std::string& dsn_id) {
    BodyState state = SpaceObjectManager::get_DSN_state_at_time(t3_tdb, dsn_id);
    Vec3d messenger_point = spline_position->interpolate(t3_tdb);
    //Vec3d messenger_point = SpaceObjectManager::get_body_state_at_time(t3_tdb, "MESSENGER");
    SpiceDouble dt = (state.position.distanceTo(messenger_point)) / C;
    SpiceDouble t2_tdb = t3_tdb - dt;

    for (int i = 0; i < 5; i++) {
        messenger_point = spline_position->interpolate(t2_tdb);
        dt = (state.position.distanceTo(messenger_point)) / C;
        t2_tdb = t3_tdb - dt;
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

Mat3d LightTimeSolver::get_spline_mat3d(SpiceDouble time_tdb) {
    return this->spline_jacobian->interpolate_mat3d(time_tdb);
}

LightTimeSolver::~LightTimeSolver() {
    delete spline_position;
    delete spline_velocity;
    delete spline_jacobian;
}
