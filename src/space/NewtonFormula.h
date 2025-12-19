#ifndef NEWTONFORMULA_H
#define NEWTONFORMULA_H

#include "SpaceObject.h"
#include "../types/ExtendedBodyState.h"
#include "../types/Vec3d.h"
#include "../types/Mat3d.h"
#include <vector>
#include "../types/HighPrecVec3d.h"
#include <deque>


class NewtonFormula : public SpaceObject {
    std::vector<SpaceObject*> force_bodies;
    ExtendedBodyState current_state;
    bool implicit_newton;

public:
    void set_use_implicit(bool implicit_newton);

private:
    [[nodiscard]] ExtendedBodyState next_step_implicit_newton(const ExtendedBodyState& current_state) const;
    [[nodiscard]] ExtendedBodyState trapezoidal_corrector_newton(const ExtendedBodyState& current_state,
                                           const ExtendedBodyState& predictor_state) const;


protected:
    double step;
    ExtendedBodyState start_state;

public:
    NewtonFormula() = default;

    NewtonFormula(const std::string& object_name, const ExtendedBodyState& start_state, SpiceDouble step);

    NewtonFormula(
        std::vector<SpaceObject*> force_bodies,
        const std::string& object_name,
        const ExtendedBodyState& start_state,
        SpiceDouble step
    );
    void set_object_name(const std::string& object_name) override;
    BodyState get_body_state(SpiceDouble tdb) override;

    [[nodiscard]] ExtendedBodyState next_step(const ExtendedBodyState& current_state) const;
    [[nodiscard]] Mat3d calculate_jacobian(SpiceDouble time, const Vec3d& position) const;

    [[nodiscard]] Vec3d calculate_acceleration(SpiceDouble time, const Vec3d& current_position) const;
    Vec3d calculate_acceleration_hp(SpiceDouble time, const Vec3d& current_position) const;
    ExtendedBodyState calculate_to_target(ExtendedBodyState current_state, SpiceDouble target_time);
    static ExtendedBodyState interpolate(const ExtendedBodyState& first, const ExtendedBodyState& second, SpiceDouble current_time);
    void set_current_body_state(const ExtendedBodyState& body_state) override;
    void add_force_body(SpaceObject* force_body);
};

#endif // NEWTONFORMULA_H
