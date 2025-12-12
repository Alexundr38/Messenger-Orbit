//
// Created by magofrays on 11/3/25.
//

#include "Simulation.h"

#include <atomic>
#include <thread>

#include "../space/NewtonFormula.h"
#include "SimulationTime.h"

bool Simulation::next_step(bool simulation_running)
{
    if (current_date < end_date && simulation_running)
    {
        for (size_t i = 0; i < spaceObjects.size(); i++) {
            spaceObjects[i]->get_body_state(current_date);
        }
        current_date += step;
    }
    return false;
}

void Simulation::run(std::atomic<bool>& simulation_running)
{
    while (next_step(simulation_running))
    {

        // std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void Simulation::set_space_objects(std::vector<SpaceObject*> spaceObjects)
{
    this->spaceObjects = std::move(spaceObjects);
}

void Simulation::add_space_object(SpaceObject* object)
{
    spaceObjects.push_back(object);
}
