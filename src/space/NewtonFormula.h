#ifndef NEWTONFORMULA_H
#define NEWTONFORMULA_H

#include "SpaceObject.h"
#include "../types/BodyState.h"
#include "../types/Vec3d.h"
#include "../types/Mat3d.h"
#include <vector>
#include <deque>

class NewtonFormula : public SpaceObject {

    std::vector<SpaceObject*> force_bodies;
    double step;
    BodyState start_state;

    bool use_implicit_methods;
    bool use_corrector;
    bool use_newton_methods;
    double stiffness_threshold;

    std::deque<BodyState> previous_states;

    BodyState next_step_implicit_newton(const BodyState& current_state) const;
    BodyState trapezoidal_corrector_newton(const BodyState& current_state,
                                          const BodyState& predictor_state) const;
    BodyState implicit_midpoint_newton(const BodyState& current_state) const;
    BodyState dopri8_newton_corrector(const BodyState& current_state) const;
    BodyState adaptive_newton_method(const BodyState& current_state) const;

    Mat3d calculate_acceleration_jacobian(SpiceDouble time, const Vec3d& position) const;
    double estimate_stiffness(const BodyState& state) const;
    BodyState next_step_explicit_euler(const BodyState& current_state) const;

protected:

    using SpaceObject::body_states;
    using SpaceObject::current_body_state;

public:
    NewtonFormula(
        std::vector<SpaceObject*> force_bodies,
        const std::string& object_name,
        const BodyState& start_state,
        SpiceDouble step
    );
    void set_object_name(const std::string& object_name) override;
    BodyState get_body_state(SpiceDouble tdb) override;

    [[nodiscard]] BodyState next_step(const BodyState& current_state) const;

    [[nodiscard]] Vec3d calculate_acceleration(SpiceDouble time, const Vec3d& current_position) const;
    BodyState calculate_to_target(BodyState current_state, SpiceDouble target_time);
    static BodyState interpolate(const BodyState& first, const BodyState& second, SpiceDouble current_time);
    static Vec3d integrate(const Vec3d& derivative, SpiceDouble step);

    void set_start_state(const BodyState& start_state);
    void add_force_body(SpaceObject* force_body);
    void set_implicit_methods(bool use_implicit);
    void set_use_corrector(bool use_corr);
    void set_newton_methods(bool use_newton);
    void set_stiffness_threshold(double threshold);

    void set_current_body_state(const BodyState& state) override;
    [[nodiscard]] BodyState get_current_body_state() const;
};

#endif // NEWTONFORMULA_H