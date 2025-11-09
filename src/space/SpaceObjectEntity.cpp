//
// Created by magofrays on 11/1/25.
//

#include "SpaceObjectEntity.h"

void SpaceObjectEntity::set_object_name(const std::string& object_name)
{
    SpaceObject::set_object_name(object_name);
    set_gravitational_parameter(SpaceObjectManager::get_body_gm(object_name));
}

SpaceObjectEntity::SpaceObjectEntity(const std::string& object_name)
{
    this->object_name = object_name;
    this->gravitational_parameter = SpaceObjectManager::get_body_gm(object_name);
}

BodyState SpaceObjectEntity::get_body_state(SpiceDouble tdb)
{
    try
    {
        BodyState new_state = SpaceObject::get_body_state(tdb);
        set_current_body_state(new_state);
        return new_state;
    }catch (std::runtime_error &e)
    {
    BodyState new_state = SpaceObjectManager::get_body_state_at_time(tdb, object_name);
    add_body_state(new_state);
    set_current_body_state(new_state);
    return new_state;
    }
}
