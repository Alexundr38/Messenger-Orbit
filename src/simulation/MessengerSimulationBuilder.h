//
// Created by magofrays on 12/12/25.
//

#ifndef MESSENGERORBIT_MESSENGERSIMULATIONBUILDER_H
#define MESSENGERORBIT_MESSENGERSIMULATIONBUILDER_H

#include "SimulationBuilder.h"


class MessengerSimulationBuilder : public SimulationBuilder
{
    std::vector<SpaceObject *> objects;
    public:
    Simulation* buildSimulation() override;
};


#endif //MESSENGERORBIT_DIAPAZONESIMULATIONBUILDER_H