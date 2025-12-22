//
// Created by magofrays on 12/19/25.
//

#ifndef MESSENGERORBIT_BOOSTSIMULATION_H
#define MESSENGERORBIT_BOOSTSIMULATION_H

#include "Simulation.h"
#include "../space/NewtonFormulaProxy.h"
#include <boost/numeric/odeint.hpp>
#include <array>

using namespace boost::numeric::odeint;

typedef std::array<long double, 6> state_type;


class BoostSimulation: public Simulation
{

    adams_bashforth_moulton<8, state_type> abm_stepper;
    runge_kutta4<state_type> rk4_stepper;
    size_t init_steps_done = 0;
    const size_t init_steps_needed = 8;
    class NewtonFormulaWrapper {
        NewtonFormula* newton_formula;

    public:
        NewtonFormulaWrapper(NewtonFormula* nf) : newton_formula(nf) {}

        void operator()(const state_type& x, state_type& dxdt, double t) const {
            dxdt[0] = x[3];
            dxdt[1] = x[4];
            dxdt[2] = x[5];
            Vec3d position{x[0], x[1], x[2]};
            Vec3d acceleration = newton_formula->calculate_acceleration(t, position);

            dxdt[3] = acceleration.x;
            dxdt[4] = acceleration.y;
            dxdt[5] = acceleration.z;
        }
    };

    NewtonFormulaWrapper wrapper;
    NewtonFormula * newton_formula;
    public:
    explicit BoostSimulation(NewtonFormula* newton_formula);
    bool next_step(bool simulation_running) override;
};


#endif //MESSENGERORBIT_BOOSTSIMULATION_H