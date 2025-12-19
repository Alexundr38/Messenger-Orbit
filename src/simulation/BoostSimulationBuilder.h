//
// Created by magofrays on 12/19/25.
//

#ifndef MESSENGERORBIT_BOOSTSIMULATIONBUILDER_H
#define MESSENGERORBIT_BOOSTSIMULATIONBUILDER_H
#include "SimulationBuilder.h"

class BoostSimulationBuilder: public SimulationBuilder
{
    public:
    Simulation* buildSimulation() override;
};


#endif //MESSENGERORBIT_BOOSTSIMULATIONBUILDER_H