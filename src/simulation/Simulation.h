//
// Created by magofrays on 11/3/25.
//

#ifndef MESSENGERORBIT_SIMULATION_H
#define MESSENGERORBIT_SIMULATION_H
#include <atomic>
#include <vector>

#include "../space/SpaceObject.h"


class NewtonFormula;

struct Simulation
{
    Simulation() = default;
    std::vector<SpaceObject*> spaceObjects;
    SpiceDouble start_date;
    SpiceDouble current_date;
    SpiceDouble end_date;
    SpiceDouble step;
    bool next_step(bool simulation_running);
    void run(std::atomic<bool>& simulation_running);
    void set_space_objects(std::vector<SpaceObject*> spaceObjects);
    void add_space_object(SpaceObject *object);


};




#endif //MESSENGERORBIT_SIMULATION_H
