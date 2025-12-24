//
// Created by sun on 23.12.2025.
//

#include "RelativeFormula.h"

RelativeFormula::RelativeFormula(const std::string object_name)
{
    this->object_name = object_name;
    SpaceObject::set_object_name(object_name);
}

//неважно
RelativeFormula::RelativeFormula(
    std::vector<SpaceObject*> force_bodies,
    const std::string & object_name
) : force_bodies(std::move(force_bodies))
{
    this->object_name = object_name;
    this->size = 100;
    SpaceObject::set_object_name(object_name);
}

//неважно
void RelativeFormula::set_object_name(const std::string& object_name) {
    SpaceObject::set_object_name(object_name);
}

//неважно
BodyState RelativeFormula::get_body_state(const SpiceDouble tdb) {
    if (body_states->empty())
    {
        throw std::invalid_argument("No BodyState in container, can't integrate");
    }
    auto result = SpaceObjectManager::get_body_state_at_time((tdb) * day, "-236");
    return result;
}

//неважно
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
        Vec3d body_pos_t2 = body->get_body_state(t2_tdb).position;
        Vec3d body_pos_t3 = body->get_body_state(t3_tdb).position;
        Vec3d dsn_pos_t3 = SpaceObjectManager::get_DSN_state_at_time(t3_tdb, dsn_id).position;
        Vec3d messenger_pos_t2 = SpaceObjectManager::get_body_state_at_time(t2_tdb, "-236").position;
        Vec3d r2 = messenger_pos_t2.absDiff(body_pos_t2);
        Vec3d r3 = dsn_pos_t3.absDiff(body_pos_t3);

        SpiceDouble gm = body->get_gravitational_parameter();
        Vec3d r23 = r2.absDiff(r3);

        SpiceDouble up = 0.0;
        SpiceDouble down = 0.0;
        if (body->get_object_name() == "SUN") {
            up += 2 * gm / (C * C /* / day / day */ );
            down += 2 * gm / (C * C /* / day / day */ );
        }
        up += r2.norm() + r3.norm() + r23.norm();
        down += r2.norm() + r3.norm() - r23.norm();
        ro += log(up/down) * (2 * gm / (C * C * C /* /day / day / day */ ));
    }
    return ro;
}