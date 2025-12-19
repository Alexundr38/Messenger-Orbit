//
// Created by magofrays on 12/19/25.
//

#ifndef MESSENGERORBIT_HIGHPRECVEC3D_H
#define MESSENGERORBIT_HIGHPRECVEC3D_H
#include "../utils/HighPrecision.h"

namespace hp {
    struct HighPrecVec3d {
        hp::real x, y, z;

        HighPrecVec3d() = default;
        HighPrecVec3d(hp::real x, hp::real y, hp::real z) : x(x), y(y), z(z) {}
        HighPrecVec3d(const Vec3d& v) : x(hp::from_double(v.x)), y(hp::from_double(v.y)), z(hp::from_double(v.z)) {}

        [[nodiscard]] hp::real norm() const {
            return boost::multiprecision::sqrt(x*x + y*y + z*z);
        }

        HighPrecVec3d operator-(const HighPrecVec3d& other) const {
            return {x - other.x, y - other.y, z - other.z};
        }

        HighPrecVec3d operator+(const HighPrecVec3d& other) const {
            return {x + other.x, y + other.y, z + other.z};
        }

        HighPrecVec3d operator*(const hp::real& scalar) const {
            return {x * scalar, y * scalar, z * scalar};
        }

        HighPrecVec3d operator/(const hp::real& scalar) const {
            return {x / scalar, y / scalar, z / scalar};
        }

        Vec3d to_vec3d() const {
            return {hp::to_double(x), hp::to_double(y), hp::to_double(z)};
        }
    };
}
#endif //MESSENGERORBIT_HIGHPRECVEC3D_H