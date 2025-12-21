//
// Created by magofrays on 12/13/25.
//

#include "MessengerSimulationAdapter.h"

#include "../utils/SpaceObjectManager.h"

MessengerSimulationAdapter::MessengerSimulationAdapter(Simulation* simulation)
{
    this->simulation = simulation;
}

// Если вдруг ломает что-то пофишку
std::unique_ptr<std::map<SpiceDouble, ExtendedBodyState>> MessengerSimulationAdapter::get_messenger_between_km(
    SpiceDouble start_date, SpiceDouble end_date, SpiceDouble step, ExtendedBodyState start_state) const
{
    auto result = simulation->simulation_between(start_date, end_date, step, "MESSENGER", start_state);
    auto first = result->find(start_date);
    auto second = result->find(end_date);
    std::unique_ptr<std::map<SpiceDouble, ExtendedBodyState>> messenger_map = std::make_unique<std::map<SpiceDouble, ExtendedBodyState>>();
    while (first != second)
    {
        ExtendedBodyState state_ssb = first->second;
        BodyState mercury = SpaceObjectManager::get_body_state_at_time(first->first*day, "MERCURY BARYCENTER");
        state_ssb.position += mercury.position;
        state_ssb.velocity += mercury.velocity;
        messenger_map->insert(std::make_pair(first->first, state_ssb));
        first = std::next(first);
    }
    return std::move(messenger_map);
}
