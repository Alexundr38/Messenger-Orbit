//
// Created by magofrays on 12/13/25.
//

#ifndef MESSENGERORBIT_SIMULATIONADAPTER_H
#define MESSENGERORBIT_SIMULATIONADAPTER_H
#include "Simulation.h"
#include "../converter/UnitsConverter.h"


class MessengerSimulationAdapter
{
    Simulation * simulation;
public:
    explicit MessengerSimulationAdapter(Simulation * simulation);
    std::unique_ptr<std::map<SpiceDouble, BodyState>> get_messenger_between_km(SpiceDouble start_date, SpiceDouble end_date, SpiceDouble step, BodyState start_state) const;
};


#endif //MESSENGERORBIT_SIMULATIONADAPTER_H