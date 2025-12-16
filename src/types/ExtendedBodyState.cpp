//
// Created by magofrays on 12/15/25.
//

#include "ExtendedBodyState.h"

Mat3d& ExtendedBodyState::getJacobian()
{ return jacobian; }

void ExtendedBodyState::setJacobian(const Mat3d& jacobian)
{
    this->jacobian = jacobian;
}

ExtendedBodyState::ExtendedBodyState(const Vec3d& position, const Vec3d& velocity, SpiceDouble time,
    const Mat3d& jacobian): BodyState(position, velocity, time),
                            jacobian(jacobian)
{
}

ExtendedBodyState::ExtendedBodyState(const Vec3d& position, const Vec3d& velocity, SpiceDouble time): BodyState(position, velocity, time),
    jacobian(Mat3d::Identity())
{
}

ExtendedBodyState::ExtendedBodyState(const BodyState& body_state): BodyState(body_state), jacobian(Mat3d::Identity())
{
}
ExtendedBodyState::ExtendedBodyState(const ExtendedBodyState& body_state): BodyState(body_state), jacobian(body_state.jacobian)
{
}