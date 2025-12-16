//
// Created by magofrays on 11/3/25.
//

#ifndef MESSENGERORBIT_SIMULATION_H
#define MESSENGERORBIT_SIMULATION_H
#include <atomic>
#include <vector>

#include "../space/SpaceObject.h"



class Simulation
{

    std::vector<SpaceObject*> spaceObjects;
    SpiceDouble start_date;
    SpiceDouble current_date;
    SpiceDouble end_date;
    SpiceDouble step;
    public:
    Simulation() = default;
    bool next_step(bool simulation_running);
    void run(bool simulation_running);
    [[nodiscard]] std::shared_ptr<std::map<SpiceDouble, ExtendedBodyState>> get_body_states(const std::string& body_name) const;
    void set_start_date(SpiceDouble date);
    void set_current_date(SpiceDouble date);
    void set_end_date(SpiceDouble date);
    void set_step(SpiceDouble step);
    [[nodiscard]] SpiceDouble get_start_date() const;
    [[nodiscard]] SpiceDouble get_current_date() const;
    [[nodiscard]] SpiceDouble get_step() const;
    [[nodiscard]] SpiceDouble get_end_date() const;
    [[nodiscard]] std::vector<SpaceObject*> get_space_objects() const;
    void set_space_objects(std::vector<SpaceObject*> spaceObjects);
    void add_space_object(SpaceObject *object);
    SpaceObject* get_object(std::string name);
    std::shared_ptr<std::map<SpiceDouble, ExtendedBodyState>> simulation_between(
        SpiceDouble start_date, SpiceDouble end_date, std::string body_name, BodyState body_state
    );
};




#endif //MESSENGERORBIT_SIMULATION_H
