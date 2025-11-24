//
// Created by magofrays on 10/31/25.
//

#include "SpaceBodyManager.h"
#include <SpiceUsr.h>

#include <utility>

#include "../utils/TimeConverter.h"
#include "../utils/PropertiesReader.h"

SpaceBodyManager::SpaceBodyManager()
{
    furnsh_c(PropertiesReader::get_property_path("forward-task", "space-body-manager", "bsp-path").c_str());
    reference_frame = PropertiesReader::get_property("forward-task", "space-body-manager", "reference-frame");
    aberration_correction = PropertiesReader::get_property("forward-task", "space-body-manager", "aberration-correction");
    observer_body = PropertiesReader::get_property("forward-task", "space-body-manager", "observer-body");
}

SpaceBodyManager& SpaceBodyManager::get_instance()
{
    static SpaceBodyManager instance;
    return instance;
}

BodyState SpaceBodyManager::get_body_state_at_time(SpiceDouble tdb, const std::string& target_body) const
{
    BodyState body_state;
    body_state.time = tdb;
    SpiceDouble state[6];
    spkezr_c(target_body.c_str(), tdb,
        reference_frame.c_str(),
        aberration_correction.c_str(),
        observer_body.c_str(),
        state,
        &body_state.light_time);
    body_state.position = {state[0], state[1], state[2]};
    body_state.velocity = {state[3], state[4], state[5]};
    return body_state;
}

BodyState SpaceBodyManager::get_body_state_at_time(const std::string& utc, const std::string& target_body) const
{
    return get_body_state_at_time(TimeConverter::to_tdb(utc), target_body);
}

