//
// Created by magofrays on 11/3/25.
//

#ifndef MESSENGERORBIT_SIMULATION_H
#define MESSENGERORBIT_SIMULATION_H
#include <vector>

#include "../space/SpaceObject.h"


struct Simulation
{
    std::vector<SpaceObject*> spaceObjects;

    SpiceDouble start_date;
    SpiceDouble end_date;
    SpiceDouble step;
    void next_step();
    void run();
};


#endif //MESSENGERORBIT_SIMULATION_H