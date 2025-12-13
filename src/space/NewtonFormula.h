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
    BodyState current_state;
    std::deque<BodyState> previous_states;

public:
    void set_use_relativistic_corrections(bool use_relativistic_corrections);

private:
    BodyState trapezoidal_corrector_newton(const BodyState& current_state,
                                          const BodyState& predictor_state) const;


protected:
    double step;
    BodyState start_state;

public:
    NewtonFormula() = default;

    NewtonFormula(const std::string& object_name, const BodyState& start_state, SpiceDouble step);

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
    void set_current_body_state(const BodyState& body_state) override;
    void add_force_body(SpaceObject* force_body);
};

#endif // NEWTONFORMULA_H
