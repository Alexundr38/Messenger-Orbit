//
// Created by magofrays on 12/19/25.
//

#include "BoostSimulation.h"

BoostSimulation::BoostSimulation(NewtonFormula * newton_formula): newton_formula(newton_formula), wrapper(newton_formula)
{}

bool BoostSimulation::next_step(bool simulation_running)
{
    if (current_date < end_date && simulation_running)
    {
        ExtendedBodyState body_state = newton_formula->get_current_body_state();
        state_type x = {body_state.position.x, body_state.position.y, body_state.position.z,
                    body_state.velocity.x,  body_state.velocity.y,  body_state.velocity.z};

        if (init_steps_done < init_steps_needed) {
            rk4_stepper.do_step(wrapper, x, current_date, step);
            init_steps_done++;
        } else {
            abm_stepper.do_step(wrapper, x, current_date, step);
        }
        Vec3d pos{x[0], x[1], x[2]};
        Vec3d vel{x[3], x[4], x[5]};
        Mat3d jacobian = body_state.jacobian*newton_formula->calculate_jacobian(current_date, pos);
        newton_formula->set_current_body_state(
            ExtendedBodyState(pos, vel, current_date, jacobian)
            );
        current_date += step;
        return true;
    }
    return false;
}

