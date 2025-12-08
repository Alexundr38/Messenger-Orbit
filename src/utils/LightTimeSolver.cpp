//
// Created by sun on 05.12.2025.
//

#include "LightTimeSolver.h"

#include "SpaceObjectManager.h"

LightTimeSolver::LightTimeSolver(std::map<SpiceDouble, BodyState> &points) {
    spline = new Spline();
    spline->build_all_splines(points);
}


SpiceDouble LightTimeSolver::light_time_solve(SpiceDouble& t3_tdb, std::string& dsn_id) {
    BodyState state = SpaceObjectManager::get_DSN_state_at_time(t3_tdb, dsn_id);
    Vec3d messenger_point = spline->interpolate(t3_tdb);
    long double dt = (state.position.distanceTo(messenger_point)) / C;
    SpiceDouble t2_tdb = t3_tdb - dt;

    for (int i = 0; i < 5; i++) {
        t2_tdb = t3_tdb - dt;
        messenger_point = spline->interpolate(t2_tdb);
        dt = (state.position.distanceTo(messenger_point)) / C;
    }

    return t2_tdb;
}

Vec3d LightTimeSolver::get_vec(SpiceDouble& t3_tdb, std::string& dsn_id)
{
    SpiceDouble t_send = light_time_solve(t3_tdb, dsn_id);
    return this->spline->interpolate(t_send) - this->spline->interpolate(t3_tdb);
}