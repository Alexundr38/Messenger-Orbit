//
// Created by magofrays on 11/4/25.
//

#ifndef MESSENGERORBIT_MESSENGERSIMULATIONBUILDER_H
#define MESSENGERORBIT_MESSENGERSIMULATIONBUILDER_H
#include "SimulationBuilder.h"


class SpaceObjectEntity;

class MessengerSimulationBuilder: public SimulationBuilder
{
    Simulation *simulation = nullptr;
    std::vector<SpaceObject*> simulation_bodies;
    std::string start_date;
    std::string step;
    std::string end_date;

    public:
    MessengerSimulationBuilder();
    Simulation* buildSimulation() override;
    ~MessengerSimulationBuilder() override;
};


#endif //MESSENGERORBIT_MESSENGERSIMULATIONBUILDER_H