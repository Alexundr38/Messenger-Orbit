//
// Created by magofrays on 11/3/25.
//

#ifndef MESSENGERORBIT_NEWTONFORMULA_H
#define MESSENGERORBIT_NEWTONFORMULA_H
#include "SpaceObject.h"


class NewtonFormula : public SpaceObject
{
    std::vector<SpaceObject*> force_bodies;
    SpiceDouble step;
    BodyState start_state;

    [[nodiscard]] BodyState next_step(const BodyState & current_state) const;
    [[nodiscard]] Vec3d calculate_acceleration(SpiceDouble time, const Vec3d& current_position) const;
    static Vec3d integrate(const Vec3d& derivative, SpiceDouble step);
    BodyState calculate_to_target(BodyState current_state, SpiceDouble target_time);
    static BodyState interpolate(const BodyState& first, const BodyState& second, SpiceDouble current_time);

public:
    NewtonFormula(std::vector<SpaceObject*> force_bodies, const std::string& object_name, const BodyState& start_state,
                  SpiceDouble step);
    void add_force_body(SpaceObject* force_body);
    void set_start_state(const BodyState& start_state);

    BodyState get_body_state(SpiceDouble tdb) override;
};


#endif //MESSENGERORBIT_NEWTONFORMULA_H