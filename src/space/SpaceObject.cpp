//
// Created by magofrays on 10/26/25.
//

#include "SpaceObject.h"

void SpaceObject::set_object_name(const std::string& object_name)
{
    this->object_name = object_name;
}

void SpaceObject::set_gravitational_parameter(SpiceDouble gravitational_parameter)
{
    this->gravitational_parameter = gravitational_parameter;
}

std::string SpaceObject::get_object_name() const
{
    return object_name;
}

SpiceDouble SpaceObject::get_gravitational_parameter() const
{
    return gravitational_parameter;
}


void SpaceObject::add_body_state(const BodyState& body_state)
{
    if (caching)
    body_states[body_state.time] = body_state;
}

BodyState SpaceObject::get_body_state(SpiceDouble tdb)
{
    if (this->body_states.find(tdb)!= this->body_states.end())
    {
        return this->body_states[tdb];
    }
    throw std::runtime_error("SpaceObject::get_body_state: body state not found");
}

