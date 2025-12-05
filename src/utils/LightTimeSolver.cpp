//
// Created by sun on 05.12.2025.
//

#include "LightTimeSolver.h"

#include "SpaceObjectManager.h"
#include "../types/BodyState.h"

LightTimeSolver::LightTimeSolver(std::map<SpiceDouble, Vec3d> &points) {
    spline = new Spline();
    spline->build_all_splines(points);
}


SpiceDouble LightTimeSolver::light_time_solve(SpiceDouble t3_tdb, long double& freq, std::string& dsn_id) {
    BodyState state = SpaceObjectManager::get_DSN_state_at_time(t3_tdb, dsn_id);
    Vec3d messenger_point = spline->interpolate(t3_tdb);
    long double dt = (state.position.distanceTo(messenger_point)) / light_speed;
    SpiceDouble t2_tdb = t3_tdb - dt;

    for (int i = 0; i < 5; i++) {
        t2_tdb = t3_tdb - dt;
        messenger_point = spline->interpolate(t2_tdb);
        dt = (state.position.distanceTo(messenger_point)) / light_speed;
    }

    return t2_tdb;
}