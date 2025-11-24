//
// Created by magofrays on 10/26/25.
//

#include "BodyState.h"

#include <utility>

BodyState::BodyState(const Vec3d& position, const Vec3d& velocity, const SpiceDouble time, const SpiceDouble light_time)
{
    this->position = position;
    this->velocity = velocity;
    this->time = time;
    this->light_time = light_time;
}
