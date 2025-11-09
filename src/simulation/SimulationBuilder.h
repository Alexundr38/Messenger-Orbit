//
// Created by magofrays on 11/3/25.
//

#ifndef MESSENGERORBIT_SIMULATIONBUILDER_H
#define MESSENGERORBIT_SIMULATIONBUILDER_H
#include "Simulation.h"


class SimulationBuilder
{
public:
    virtual ~SimulationBuilder() = default;
    virtual Simulation* buildSimulation() = 0;
};


#endif //MESSENGERORBIT_SIMULATIONBUILDER_H