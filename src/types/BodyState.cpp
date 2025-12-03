//
// Created by magofrays on 10/26/25.
//

#include "BodyState.h"

#include <utility>

BodyState::BodyState(const Vec3d& position, const Vec3d& velocity, const SpiceDouble time)
{
    this->position = position;
    this->velocity = velocity;
    this->time = time;
}

std::ostream& operator<<(std::ostream& os, const BodyState& state)
{
    os << std::fixed << std::setprecision(12);
    os << "Time: " << state.time << " TDB\n";
    os << "Position: (" << state.position.x << ", "
        << state.position.y << ", " << state.position.z << ") km\n";
    os << "Velocity: (" << state.velocity.x << ", "
        << state.velocity.y << ", " << state.velocity.z << ") km/s";
    return os;
}
