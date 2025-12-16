//
// Created by magofrays on 12/13/25.
//

#include "simulation/MessengerSimulationAdapter.h"
#include "simulation/MessengerSimulationBuilder.h"
#include "simulation/Simulation.h"

int main(int argc, char* argv[])
{
    Simulation *simulation = MessengerSimulationBuilder().buildSimulation();
    MessengerSimulationAdapter adapter(simulation);
    // simulation->run(true);
    ExtendedBodyState start_body_state(
    Vec3d(9.550067806899118E+05,  4.077510763843564E+07,  2.161674651436035E+07),
    Vec3d(-5.951679809132351E+01,  1.883581016968700E+00,  3.915801068188813E+00),
    353682600
    );
    auto result = adapter.get_messenger_between_km(353682600, 353937600, 1, start_body_state);
    std::cout << result.get() << std::endl;
    delete simulation;
    return 0;
}
