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
        return true;
    }
    return false;
}

void Simulation::run(bool simulation_running)
{
    while (next_step(simulation_running))
    {

    }
}

std::shared_ptr<std::map<SpiceDouble, ExtendedBodyState>> Simulation::get_body_states(const std::string& body_name) const
{
    for (auto object : spaceObjects)
    {
        if (object->get_object_name() == body_name)
        {
            return object->get_body_states();
        }
    }
    throw std::invalid_argument("body name not found");
}

void Simulation::set_start_date(const SpiceDouble date)
{
    this->start_date = date;
}

void Simulation::set_current_date(const SpiceDouble date)
{
    this->current_date = date;
}

void Simulation::set_end_date(const SpiceDouble date)
{
    this->end_date = date;
}

void Simulation::set_step(const SpiceDouble step)
{
    this->step = step;
}

SpiceDouble Simulation::get_start_date() const
{
    return this->start_date;
}

SpiceDouble Simulation::get_current_date() const
{
    return this->current_date;
}

SpiceDouble Simulation::get_step() const
{
    return this->step;
}

SpiceDouble Simulation::get_end_date() const
{
    return this->end_date;
}

std::vector<SpaceObject*> Simulation::get_space_objects() const
{
    return spaceObjects;
}

void Simulation::set_space_objects(std::vector<SpaceObject*> spaceObjects)
{
    this->spaceObjects = std::move(spaceObjects);
}

void Simulation::add_space_object(SpaceObject* object)
{
    spaceObjects.push_back(object);
}

SpaceObject* Simulation::get_object(std::string name)
{
    for (auto object : spaceObjects)
    {
        if (object->get_object_name() == name)
        {
            return object;
        }
    }
    throw std::invalid_argument("object not found");
}


std::shared_ptr<std::map<SpiceDouble, ExtendedBodyState>> Simulation::simulation_between(SpiceDouble start_date,
                                                                                 SpiceDouble end_date, SpiceDouble step, std::string body_name, ExtendedBodyState body_state)
{
    set_start_date(start_date);
    set_current_date(start_date);
    set_end_date(end_date);
    set_step(step);
    NewtonFormula * needed_object = static_cast<NewtonFormula*>(get_object(body_name));
    needed_object->set_current_body_state(body_state);
    run(true);
    return needed_object->get_body_states();
}
