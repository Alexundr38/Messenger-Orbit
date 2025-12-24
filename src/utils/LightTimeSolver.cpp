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

    this->relative_formula = new RelativeFormula("MESSENGER");
    relative_formula->add_force_body(new SpaceObjectEntity("SUN"));
    relative_formula->add_force_body(new SpaceObjectEntity("MERCURY BARYCENTER"));
    relative_formula->add_force_body(new SpaceObjectEntity("VENUS BARYCENTER"));
    relative_formula->add_force_body(new SpaceObjectEntity("EARTH BARYCENTER"));
    relative_formula->add_force_body(new SpaceObjectEntity("MARS BARYCENTER"));
    relative_formula->add_force_body(new SpaceObjectEntity("JUPITER BARYCENTER"));
    relative_formula->add_force_body(new SpaceObjectEntity("SATURN BARYCENTER"));
    relative_formula->add_force_body(new SpaceObjectEntity("URANUS BARYCENTER"));
    relative_formula->add_force_body(new SpaceObjectEntity("NEPTUNE BARYCENTER"));
}

SpiceDouble LightTimeSolver::light_time_solve(SpiceDouble& t3_utc, std::string& dsn_id, SpiceDouble* tdb_tai_t2)  {

    SpiceDouble t3_utc_j2000 = (t3_utc - 18262.5) * day;

    SpiceDouble delt;
    deltet_c(t3_utc_j2000, "UTC", &delt);
    SpiceDouble t3_tdb_j2000 = t3_utc_j2000 + delt;


    //SpiceDouble jd_t3tdb = unitim_c ( t3_tdb_j2000, "TDB", "JDTDB");

    BodyState state = SpaceObjectManager::get_DSN_state_at_time(t3_tdb_j2000, dsn_id);
    //BodyState earth = SpaceObjectManager::get_body_state_at_time(t3_tdb_j2000, "EARTH");


    Vec3d messenger_point;
    SpiceDouble dt;
    SpiceDouble t2_tdb = t3_tdb_j2000;

    for (int i = 0; i < 10; i++) {
        //messenger_point = SpaceObjectManager::get_body_state_at_time(t2_tdb, "-236").position;
        messenger_point = spline_position->interpolate(t2_tdb/day);
        /*dt = std::sqrt((state.position.x - messenger_point.position.x) *
                               (state.position.x - messenger_point.position.x) +

                               (state.position.y - messenger_point.position.y) *
                               (state.position.y - messenger_point.position.y) +

                               (state.position.z - messenger_point.position.z) *
                               (state.position.z - messenger_point.position.z)  ) / C;*/
        dt = (state.position.distanceTo(messenger_point)) / C;
        t2_tdb = t3_tdb_j2000 - dt * day - relative_formula->get_relative_ro(t2_tdb, t3_tdb_j2000, dsn_id);
    }

    SpiceDouble delt2;
    deltet_c(t2_tdb, "ET", &delt2);
    SpiceDouble t2_utc = t2_tdb - delt2;

    SpiceDouble t2_tai = t2_utc + 35.0;

    *tdb_tai_t2 = (t2_tdb - t2_tai) / day;

    return (t3_utc_j2000 - t2_tdb) / day;


    /*BodyState state = SpaceObjectManager::get_DSN_state_at_time(t3_tdb, dsn_id);
    Vec3d messenger_point = spline_position->interpolate(t3_tdb);
    //Vec3d messenger_point = SpaceObjectManager::get_body_state_at_time(t3_tdb, "MESSENGER");
    SpiceDouble dt = (state.position.distanceTo(messenger_point)) / C;
    SpiceDouble t2_tdb = t3_tdb - dt;

    for (int i = 0; i < 5; i++) {
        messenger_point = spline_position->interpolate(t2_tdb);
        dt = (state.position.distanceTo(messenger_point)) / C;
        t2_tdb = t3_tdb - dt;
    }

    return t2_tdb;*/
}

Vec3d LightTimeSolver::get_vec_2_3(SpiceDouble& t3_tdb, std::string& dsn_id)
{
    SpiceDouble tdb_tai;
    SpiceDouble t_send = light_time_solve(t3_tdb, dsn_id, &tdb_tai);
    //SpiceDouble t_send = light_time_solve(t3_tdb, dsn_id);
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
