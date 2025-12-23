//
// Created by sun on 23.12.2025.
//

#include "RelativeFormula.h"

RelativeFormula::RelativeFormula(const std::string object_name)
{
    this->object_name = object_name;
    SpaceObject::set_object_name(object_name);
}

RelativeFormula::RelativeFormula(
    std::vector<SpaceObject*> force_bodies,
    const std::string & object_name
) : force_bodies(std::move(force_bodies))
{
    this->object_name = object_name;
    this->size = 100;
    SpaceObject::set_object_name(object_name);
}

void RelativeFormula::set_object_name(const std::string& object_name) {
    SpaceObject::set_object_name(object_name);
}

BodyState RelativeFormula::get_body_state(const SpiceDouble tdb) {
    if (body_states->empty())
    {
        throw std::invalid_argument("No BodyState in container, can't integrate");
    }
    auto result = SpaceObjectManager::get_body_state_at_time((tdb) * day, "-236");
    return result;
}

void RelativeFormula::set_current_body_state(const ExtendedBodyState& body_state)
{
    Renderable::set_current_body_state(body_state);
    (*body_states)[body_state.time] = body_state;
}

void RelativeFormula::add_force_body(SpaceObject* force_body) {
    force_bodies.push_back(force_body);
}

SpiceDouble RelativeFormula::get_relative_ro(SpiceDouble t2_tdb, SpiceDouble t3_tdb, std::string& dsn_id) {
    SpiceDouble ro = 0.0;
    for (SpaceObject * body : force_bodies)
    {
        Vec3d body_pos_ssb = body->get_body_state(t2_tdb).position;
        Vec3d dsn_pos_ssb = SpaceObjectManager::get_DSN_state_at_time(t3_tdb, dsn_id).position;
        SpiceDouble gm = body->get_gravitational_parameter();
        Vec3d diff = body_pos_ssb.absDiff(dsn_pos_ssb);

        SpiceDouble up = 0.0;
        SpiceDouble down = 0.0;
        if (body->get_object_name() == "SUN") {
            up += 2 * gm / (C * C);// / day / day);
            down += 2 * gm / (C * C);// / day / day);
        }
        up += body_pos_ssb.norm() + dsn_pos_ssb.norm() + diff.norm();
        down += body_pos_ssb.norm() + dsn_pos_ssb.norm() - diff.norm();
        ro += log(up/down) * (2 * gm / (C * C * C ));///day / day / day));
    }
    return ro;
}