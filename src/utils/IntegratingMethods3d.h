//
// Created by magofrays on 11/3/25.
//

#ifndef MESSENGERORBIT_INTEGRATINGMETHODS_H
#define MESSENGERORBIT_INTEGRATINGMETHODS_H
#include <functional>
#include <SpiceZdf.h>

#include "../types/BodyState.h"
#include "../types/Vec3d.h"

namespace IntegratingMethods3d
{
    Vec3d dopri8_method(double h, const std::function<Vec3d(Vec3d)>& vec_function, const Vec3d& pos);
    Vec3d rk4_method(double h, const std::function<Vec3d(Vec3d, double)>& vec_function, const Vec3d& pos, double t);
    Vec3d euler_method(double h, const std::function<Vec3d(Vec3d, double)>& vec_function, const Vec3d& pos);
    BodyState rk4_system_method(
    double h,
    const std::function<BodyState(const BodyState&)>& system_derivative,
   const BodyState& current_state);
};


#endif //MESSENGERORBIT_INTEGRATINGMETHODS_H