//
// Created by magofrays on 10/31/25.
//

#include "SpaceObjectManager.h"
#include <SpiceUsr.h>

#include <utility>

#include "TimeConverter.h"
#include "PropertiesReader.h"

SpaceObjectManager::SpaceObjectManager()
{
    furnsh_c(PropertiesReader::get_property_path("forward-task", "space-object-manager", "bsp-path").c_str());
    std::cout << PropertiesReader::get_property_path("forward-task", "space-object-manager", "tpc-path").c_str() << std::endl;
    furnsh_c(PropertiesReader::get_property_path("forward-task", "space-object-manager", "tpc-path").c_str());
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
    auto instance = get_instance();
    BodyState body_state;
    body_state.time = tdb;
    SpiceDouble state[6];
    SpiceDouble lt;
    spkezr_c(target_body.c_str(), tdb,
        instance.reference_frame.c_str(),
        instance.aberration_correction.c_str(),
        instance.observer_body.c_str(),
        state,
        &lt);
    body_state.position = {state[0], state[1], state[2]};
    body_state.velocity = {state[3], state[4], state[5]};
    return body_state;
}

BodyState SpaceObjectManager::get_body_state_at_time(const std::string& utc, const std::string& target_body)
{
    return get_body_state_at_time(TimeConverter::to_tdb(utc), target_body);
}

SpiceDouble SpaceObjectManager::get_body_gm(const std::string& target_body)
{
    auto instance = get_instance();
    SpiceInt dim;
    SpiceDouble gm[1];
    bodvrd_c(target_body.c_str(), "GM", 1, &dim, gm);
    return gm[0];
}

