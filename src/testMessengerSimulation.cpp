//
// Created by magofrays on 12/13/25.
//

#include "simulation/MessengerSimulationAdapter.h"
#include "simulation/MessengerSimulationBuilder.h"
#include "simulation/Simulation.h"
#include "utils/LSM.h"
#include "types/StateVector.h"

int main(int argc, char* argv[])
{
    Simulation *simulation = MessengerSimulationBuilder().buildSimulation();
    MessengerSimulationAdapter adapter(simulation);
    // simulation->run(true);
    ExtendedBodyState start_body_state(
    Vec3d(-1.132738182132099E-01, -4.065648693088634E-01, -2.053770547787668E-01),
    Vec3d(2.028347765177638E-02, -3.414502168970862E-03, -5.274305280690317E-03),
    1988381908 / day
    );
    for (int i = 0; i < 3; i++) {
        std::cout << std::setprecision(17) << start_body_state.position << std::endl;
        std::cout << std::setprecision(17) << start_body_state.velocity << std::endl;
        auto result = adapter.get_messenger_between_km(1988381908/day, 1988384378/day, 1/day, start_body_state);
        StateVector state = StateVector();
        state.add_state(BodyState(start_body_state.position, start_body_state.velocity, start_body_state.time));
        auto last_result = (*result)[1988384378/day];
        LSM* lsm = new LSM(state, *result, "2013.csv", 1988381908/day, 1988384378/day);
        StateVector out_state = lsm->do_LSM(2);
        start_body_state.position = out_state.state.position;
        start_body_state.velocity = out_state.state.velocity;
        std::cout << "-----------------------------------------------\n";
        delete lsm;
    }

    delete simulation;
    return 0;
}
