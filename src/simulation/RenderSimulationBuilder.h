//
// Created by magofrays on 11/4/25.
//

#ifndef MESSENGERORBIT_RenderSimulationBuilder_H
#define MESSENGERORBIT_RenderSimulationBuilder_H
#include "SimulationBuilder.h"



class RenderSimulationBuilder: public SimulationBuilder
{
    Simulation *simulation = nullptr;
    std::vector<SpaceObject*> simulation_bodies;
    std::string start_date;
    std::string step;
    std::string end_date;

    public:
    RenderSimulationBuilder();
    Simulation* buildSimulation() override;
    ~RenderSimulationBuilder() override;
};


#endif //MESSENGERORBIT_MESSENGERSIMULATIONBUILDER_H