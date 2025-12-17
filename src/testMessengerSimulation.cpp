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
    Vec3d(-1.694552201075508E+07, -6.082123875002974E+07, -3.072397008554077E+07),
    Vec3d(3.511996605436091E+01, -5.912063124751997E+00, -9.132231937650893E+00),
    1988381908
    );
    for (int i = 0; i < 10; i++) {
        auto result = adapter.get_messenger_between_km(1988381908, 1988384378, 1, start_body_state);
        StateVector state = StateVector();
        state.add_state(BodyState(start_body_state.position, start_body_state.velocity, start_body_state.time));
        LSM* lsm = new LSM(state, *result, "2013.csv", 1988381908, 1988384378);
        StateVector out_state = lsm->do_LSM();
        std::cout << out_state.state.position << std::endl;
        std::cout << out_state.state.velocity << std::endl;
        start_body_state.position = out_state.state.position;
        start_body_state.velocity = out_state.state.velocity;
        std::cout << "-----------------------------------------------\n";
    }

    delete simulation;
    return 0;
}
