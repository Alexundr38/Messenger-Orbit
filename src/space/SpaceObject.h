//
// Created by magofrays on 10/26/25.
//

#ifndef MESSENGERORBIT_BODYDATA_H
#define MESSENGERORBIT_BODYDATA_H
#include <map>
#include <vector>

#include "../render/Renderable.h"
#include "../types/BodyState.h"

class SpaceObject : public Renderable
{
    protected:
    std::map<SpiceDouble, BodyState> body_states;
    std::string object_name;
    bool caching = std::stoi(PropertiesReader::get_property("forward-task", "simulation", "caching"));
    SpiceDouble gravitational_parameter = 0;

public:
    virtual ~SpaceObject() = default;
    virtual void set_object_name(const std::string& object_name);
    void set_gravitational_parameter(SpiceDouble gravitational_parameter);
    [[nodiscard]] std::string get_object_name() const;
    [[nodiscard]] SpiceDouble get_gravitational_parameter() const;
    [[nodiscard]] std::vector<BodyState> get_body_states() const;
    void add_body_state(const BodyState& body_state);
    virtual BodyState get_body_state(SpiceDouble tdb);
};


#endif //MESSENGERORBIT_BODYDATA_H