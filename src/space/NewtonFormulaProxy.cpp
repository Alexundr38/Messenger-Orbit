//
// Created by magofrays on 12/3/25.
//

#include "NewtonFormulaProxy.h"

#include "../converter/UnitsConverter.h"

NewtonFormulaProxy::NewtonFormulaProxy(const std::vector<SpaceObject*>& force_bodies, const std::string& object_name,
                                       const ExtendedBodyState& start_state, SpiceDouble step, std::string filename) : NewtonFormula(
    force_bodies, object_name, ExtendedBodyState(start_state.position, start_state.velocity, start_state.time / day),
    step / day)
{
    start_date = start_state.time;
    filename = PathResolver::get_data(filename);
    csv_file.open(filename);
    csv_file << "x,y,z,vx,vy,vz\n";
}

NewtonFormulaProxy::NewtonFormulaProxy(const std::string& object_name,
    const ExtendedBodyState& start_state, SpiceDouble step, std::string filename) : NewtonFormula(
    object_name, ExtendedBodyState(start_state.position, start_state.velocity, start_state.time / day),
    step / day)
{
    start_date = start_state.time;
    filename = PathResolver::get_data(filename);
    csv_file.open(filename);
    csv_file << "x,y,z,vx,vy,vz\n";
}

BodyState NewtonFormulaProxy::get_body_state(SpiceDouble tdb)
{
    auto state = NewtonFormula::get_body_state(tdb/day);
    if ((int)(tdb - start_date) % 60 == 0)
    {
        csv_file << std::scientific << std::uppercase << std::setprecision(15);
        csv_file << state.position.x << ","
                    << state.position.y << ","
                    << state.position.z << ","
                    << state.velocity.x << ","
                    << state.velocity.y << ","
                    << state.velocity.z << std::endl;
    }
    return state;
}

// void NewtonFormulaProxy::set_current_body_state(const BodyState& body_state)
// {
//     NewtonFormula::set_current_body_state(UnitsConverter::toAuDay(body_state));
// }
