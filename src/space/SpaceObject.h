//
// Created by magofrays on 10/26/25.
//

#ifndef MESSENGERORBIT_BODYDATA_H
#define MESSENGERORBIT_BODYDATA_H
#include <map>
#include <vector>
#include "../utils/Constants.h"
#include "../types/BodyState.h"

struct SpiceDoubleCompare {
    bool operator()(const SpiceDouble a, const SpiceDouble b) const {
        return a < b - DP_TOL;
    }
};

class SpaceObject
{
    protected:
    std::map<SpiceDouble, BodyState, SpiceDoubleCompare> body_states;
    std::string object_name;
    SpiceDouble gravitational_parameter = 0;

public:
    void set_object_name(const std::string& object_name);

    void set_gravitational_parameter(SpiceDouble gravitational_parameter);

    [[nodiscard]] std::string get_object_name() const;

    [[nodiscard]] SpiceDouble get_gravitational_parameter() const;

    [[nodiscard]] std::vector<BodyState> get_body_states() const;
    void add_body_state(const BodyState& body_state);
    BodyState get_body_state(SpiceDouble tdb);
};


#endif //MESSENGERORBIT_BODYDATA_H