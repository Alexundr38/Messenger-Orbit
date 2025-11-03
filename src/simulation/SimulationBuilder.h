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

private:
    virtual Simulation buildSimulation();
};


#endif //MESSENGERORBIT_SIMULATIONBUILDER_H