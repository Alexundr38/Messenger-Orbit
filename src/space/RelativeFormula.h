//
// Created by sun on 23.12.2025.
//

#ifndef MESSENGERORBIT_RELATIVEFORMULA_H
#define MESSENGERORBIT_RELATIVEFORMULA_H
#include "SpaceObject.h"
#include "../utils/SpaceObjectManager.h"


class RelativeFormula: public SpaceObject {

    std::vector<SpaceObject*> force_bodies;
public:

    RelativeFormula() = default;

    RelativeFormula(const std::string object_name);

    RelativeFormula(
        std::vector<SpaceObject*> force_bodies,
        const std::string& object_name
    );
    void set_object_name(const std::string& object_name) override;
    BodyState get_body_state(SpiceDouble tdb) override;
    void set_current_body_state(const ExtendedBodyState& body_state) override;
    void add_force_body(SpaceObject* force_body);

    double get_relative_ro(double t2_tdb, double t3_tdb, std::string& dsn_id);
};

#endif //MESSENGERORBIT_RELATIVEFORMULA_H