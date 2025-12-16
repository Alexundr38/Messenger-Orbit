//
// Created by magofrays on 12/15/25.
//

#ifndef MESSENGERORBIT_EXTENDEDBODYSTATE_H
#define MESSENGERORBIT_EXTENDEDBODYSTATE_H
#include "BodyState.h"
#include "Mat3d.h"


struct ExtendedBodyState: public BodyState
{
    Mat3d jacobian;
    Mat3d& getJacobian();
    ExtendedBodyState() = default;
    void setJacobian(const Mat3d& jacobian);
    ExtendedBodyState(const Vec3d& position, const Vec3d& velocity, SpiceDouble time, const Mat3d& jacobian);
    ExtendedBodyState(const Vec3d& position, const Vec3d& velocity, SpiceDouble time);
    explicit ExtendedBodyState(const BodyState &body_state);
    ExtendedBodyState(const ExtendedBodyState& body_state);
};


#endif //MESSENGERORBIT_EXTENDEDBODYSTATE_H