//
// Created by magofrays on 11/3/25.
//

#include "Simulation.h"

#include <atomic>
#include <thread>

#include "../space/NewtonFormula.h"


void Simulation::next_step()
{

    for (size_t i = 0; i < spaceObjects.size(); i++) {
        spaceObjects[i]->get_body_state(current_date);
    }
    current_date += step;
}

void Simulation::run(std::atomic<bool>& simulation_running)
{
    while (current_date < end_date && simulation_running)
    {
        next_step();
        // std::this_thread::sleep_for(std::chrono::milliseconds(10));
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
