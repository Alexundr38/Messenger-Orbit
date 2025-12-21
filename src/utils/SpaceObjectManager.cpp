//
// Created by magofrays on 10/31/25.
//

#include "SpaceObjectManager.h"
#include <SpiceUsr.h>

#include <utility>

#include "Constants.h"
#include "HighPrecision.h"
#include "SpiceGuard.h"
#include "TimeConverter.h"
#include "PropertiesReader.h"

SpaceObjectManager::SpaceObjectManager()
{
    furnsh_c(PropertiesReader::get_property_path("forward-task", "space-object-manager", "bsp-path").c_str());
    furnsh_c(PropertiesReader::get_property_path("forward-task", "space-object-manager", "tpc-path").c_str());
    furnsh_c(PropertiesReader::get_property_path("forward-task", "space-object-manager", "dsn-coords").c_str());
    furnsh_c(PropertiesReader::get_property_path("forward-task", "space-object-manager", "add-dsn-coords").c_str());
    furnsh_c(PropertiesReader::get_property_path("forward-task", "space-object-manager", "sizes").c_str());
    reference_frame = PropertiesReader::get_property("forward-task", "space-object-manager", "reference-frame");
    aberration_correction = PropertiesReader::get_property("forward-task", "space-object-manager", "aberration-correction");
    observer_body = PropertiesReader::get_property("forward-task", "space-object-manager", "observer-body");
}

SpaceObjectManager& SpaceObjectManager::get_instance()
{
    static SpaceObjectManager instance;
    return instance;
}

BodyState SpaceObjectManager::get_body_state_at_time(SpiceDouble tdb, const std::string& target_body)
{
    std::lock_guard<std::mutex> lock(spice_mutex);
    auto instance = get_instance();
    BodyState body_state;

    SpiceDouble et_j2000, et_j1950;
    SpiceDouble j2000_to_j1950;
    str2et_c("2000 JAN 01 12:00:00 TDB", &et_j2000);
    str2et_c("1950 JAN 01 00:00:00 TDB", &et_j1950);
    j2000_to_j1950 = et_j2000 - et_j1950;

    body_state.time = tdb;
    SpiceDouble state[6];
    SpiceDouble lt;
    spkezr_c(target_body.c_str(), tdb - j2000_to_j1950,
        instance.reference_frame.c_str(),
        instance.aberration_correction.c_str(),
        instance.observer_body.c_str(),
        state,
        &lt);
    body_state.position = {state[0], state[1], state[2]};
    body_state.velocity = {state[3], state[4], state[5]};
    body_state.position /= au;
    body_state.velocity = (body_state.velocity*day)/au;
    return body_state;
}

BodyState SpaceObjectManager::get_body_state_at_time(SpiceDouble tdb, const std::string& target_body,
    const std::string& observer_body)
{
    std::lock_guard<std::mutex> lock(spice_mutex);
    auto instance = get_instance();
    BodyState body_state;

    SpiceDouble et_j2000, et_j1950;
    SpiceDouble j2000_to_j1950;
    str2et_c("2000 JAN 01 12:00:00 TDB", &et_j2000);
    str2et_c("1950 JAN 01 00:00:00 TDB", &et_j1950);
    j2000_to_j1950 = et_j2000 - et_j1950;


    body_state.time = tdb;
    SpiceDouble state[6];
    SpiceDouble lt;
    spkezr_c(target_body.c_str(), tdb - j2000_to_j1950,
        instance.reference_frame.c_str(),
        instance.aberration_correction.c_str(),
        observer_body.c_str(),
        state,
        &lt);
    body_state.position = {state[0], state[1], state[2]};
    body_state.velocity = {state[3], state[4], state[5]};
    body_state.position = body_state.position / au;
    body_state.velocity = (body_state.velocity)/au;
    return body_state;
}

BodyState SpaceObjectManager::get_body_state_at_time(const std::string& utc, const std::string& target_body)
{
    return get_body_state_at_time(TimeConverter::to_tdb(utc), target_body);
}

BodyState SpaceObjectManager::get_DSN_state_at_time(SpiceDouble tdb, const std::string& dsn_id) {
    tdb *= day;
    std::lock_guard<std::mutex> lock(spice_mutex);
    auto instance = get_instance();
    std::string dsn_name = "DSS-" + dsn_id;
    BodyState body_state;



    SpiceDouble et_j2000, et_j1950;
    SpiceDouble j2000_to_j1950;
    str2et_c("2000 JAN 01 12:00:00 TDB", &et_j2000);
    str2et_c("1950 JAN 01 00:00:00 TDB", &et_j1950);
    j2000_to_j1950 = et_j2000 - et_j1950;


    body_state.time = tdb;


    SpiceDouble state[6];
    SpiceDouble lt;
    spkezr_c(
        dsn_name.c_str(),
        tdb - j2000_to_j1950 + 63195,
        "J2000",
        "CN+S",
        "SOLAR SYSTEM BARYCENTER",
        state,
        &lt
    );

    body_state.position = {state[0] / au, state[1] / au, state[2] / au};
    body_state.velocity = {state[3] / au * day, state[4] / au * day, state[5] / au * day};
    return body_state;
}

SpiceDouble SpaceObjectManager::get_body_gm(const std::string& target_body)
{
    std::lock_guard<std::mutex> lock(spice_mutex);
    auto instance = get_instance();
    static const hp::real CONV_FACTOR = hp::get_conversion_factor();

    SpiceInt dim;
    SpiceDouble gm_km3_s2[1];
    bodvrd_c(target_body.c_str(), "GM", 1, &dim, gm_km3_s2);
    hp::real gm = hp::from_double(gm_km3_s2[0]);
    hp::real mass_ratio = gm * CONV_FACTOR;
    return hp::to_double(mass_ratio);
}


SpiceDouble SpaceObjectManager::get_body_radius(const std::string& target_body)
{
    std::lock_guard<std::mutex> lock(spice_mutex);
    auto instance = get_instance();
    std::string body_name = target_body.substr(0, target_body.find(' '));
    SpiceInt dim;
    SpiceDouble radii[3];
    bodvrd_c(body_name.c_str(), "RADII", 3, &dim, radii);
    return (radii[0] + radii[1]) / 2.0;
}

