//
// Created by magofrays on 12/13/25.
//

#include "MessengerSimulationAdapter.h"

MessengerSimulationAdapter::MessengerSimulationAdapter(Simulation* simulation)
{
    this->simulation = simulation;
}

// Если вдруг ломает что-то пофишку
std::unique_ptr<std::map<SpiceDouble, BodyState>> MessengerSimulationAdapter::get_messenger_between_km(
    SpiceDouble start_date, SpiceDouble end_date, SpiceDouble step, BodyState start_state) const
{
    auto result = simulation->simulation_between(start_date, end_date, "MESSENGER", UnitsConverter::toAuDay(start_state));
    auto first = result->find(start_date/day);
    auto second = result->find(end_date/day);
    std::unique_ptr<std::map<SpiceDouble, BodyState>> messenger_map = std::make_unique<std::map<SpiceDouble, BodyState>>();
    while (first != second)
    {
        auto temp = UnitsConverter::toKmSec(*first);
        if ((int)(temp.first - start_date) % (int)step == 0)
        {
            messenger_map->insert(temp);
        }
        first = std::next(first);
    }
    return std::move(messenger_map);
}
