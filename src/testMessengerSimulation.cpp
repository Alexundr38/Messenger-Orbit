//
// Created by magofrays on 12/13/25.
//

#include "simulation/BoostSimulationBuilder.h"
#include "simulation/MessengerSimulationAdapter.h"
#include "simulation/MessengerSimulationBuilder.h"
#include "simulation/Simulation.h"
#include "utils/LSM.h"
#include "types/StateVector.h"

int main(int argc, char* argv[])
{
    Simulation *simulation = BoostSimulationBuilder().buildSimulation();
    // simulation->run(true);
    // return 0;
    SpiceDouble start_time = 1988381908;
    SpiceDouble end_time = 1988384378;
    MessengerSimulationAdapter adapter(simulation);
    ExtendedBodyState start_body_state(
    Vec3d(-1.132738182132099E-01, -4.065648693088634E-01, -2.053770547787668E-01),
    Vec3d(2.028347765177638E-02, -3.414502168970862E-03, -5.274305280690317E-03),
    start_time / day
    );
    StateVector state = StateVector();
    StateVector out_state;
    for (int i = 0; i < 5; i++) {
        std::cout << "cur_pos: " << std::setprecision(17) << start_body_state.position << std::endl;
        std::cout << "cur_vel: " << std::setprecision(17) << start_body_state.velocity << std::endl;
        auto result = adapter.get_messenger_between_km(start_time/day, end_time/day, 10/day, start_body_state);
        state.add_state(BodyState(start_body_state.position, start_body_state.velocity, start_body_state.time));
        LSM* lsm = new LSM(state, *result, "2013.csv", start_time/day, end_time/day);
        out_state = lsm->do_LSM(3);
        std::cout << "cur_pos: " << std::setprecision(17) << out_state.state.position << std::endl;
        std::cout << "new_pos: " << std::setprecision(17) << start_body_state.position << std::endl;
        start_body_state.position = out_state.state.position;
        start_body_state.velocity = out_state.state.velocity;
        std::cout << "----------------------------------------------------------------------------------------------\n";
        delete lsm;
    }

    /*(-0.11327381821320991, -0.40656486930886337, -0.20537705477876680)
    (0.02028347765177638, -0.00341450216897086, -0.00527430528069032)

    (-0.11327343862093999, -0.40656638033640327, -0.20537424549056479)
    (0.02028347765177638, -0.00341450216897086, -0.00527430528069032)

    (-0.11327343862093999, -0.40656638033640327, -0.20537424549056479)
    (0.02028347765177638, -0.00341450216897086, -0.00527430528069032)*/

    delete simulation;
    return 0;
}


/*
SpiceDouble start_time = 2011020795;
    SpiceDouble end_time = 2011023800;
    MessengerSimulationAdapter adapter(simulation);
    ExtendedBodyState start_body_state(
    Vec3d(-1.517223966758399E-01, -3.970536370792114E-01, -1.961882991612178E-01),
    Vec3d(2.009642721040850E-02, -6.547422135645379E-03, -4.035817111972329E-03),
    start_time / day
    );
 */

//-0.11327381821320991, -0.40656486930886337, -0.20537705477876680
//-0.11327381668142248, -0.40656487040112670, -0.20537704700866269