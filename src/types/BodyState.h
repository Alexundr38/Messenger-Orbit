//
// Created by magofrays on 10/26/25.
//

#ifndef MESSENGERORBIT_BODY_STATE_H
#define MESSENGERORBIT_BODY_STATE_H
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <SpiceZdf.h>

#include "Vec3d.h"


struct BodyState
{
    Vec3d position;
    Vec3d velocity;
    SpiceDouble time;

    BodyState() : position(), velocity(), time()
    {
    }

    BodyState(const Vec3d& position, const Vec3d& velocity, SpiceDouble time);
    friend std::ostream& operator<<(std::ostream& os, const BodyState& state);
};


#endif //MESSENGERORBIT_ORBIT_BODY_STATE_H