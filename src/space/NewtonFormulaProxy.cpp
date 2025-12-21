//
// Created by magofrays on 12/3/25.
//

#include "NewtonFormulaProxy.h"

#include "../converter/UnitsConverter.h"
#include "../utils/TimeConverter.h"

NewtonFormulaProxy::NewtonFormulaProxy(const std::vector<SpaceObject*>& force_bodies, const std::string& object_name,
                                       const ExtendedBodyState& start_state, SpiceDouble step, std::string filename) : NewtonFormula(
    force_bodies, object_name, ExtendedBodyState(start_state.position, start_state.velocity, start_state.time / day),
    step / day)
{
    delimiter =  60/(int)TimeConverter::from_string_to_tdb(PropertiesReader::get_property("forward-task", "simulation", "step"));
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
    delimiter =  60/(int)TimeConverter::from_string_to_tdb(PropertiesReader::get_property("forward-task", "simulation", "step"));
    start_date = start_state.time;
    filename = PathResolver::get_data(filename);
    csv_file.open(filename);
    csv_file << "x,y,z,vx,vy,vz\n";
}

BodyState NewtonFormulaProxy::get_body_state(SpiceDouble tdb)
{
    return NewtonFormula::get_body_state(tdb/day);
}

void NewtonFormulaProxy::set_current_body_state(const ExtendedBodyState& body_state)
{
    NewtonFormula::set_current_body_state(body_state);
    if (counter % delimiter == 0)
    {
        csv_file << std::scientific << std::uppercase << std::setprecision(15);
        csv_file << body_state.position.x << ","
                    << body_state.position.y << ","
                    << body_state.position.z << ","
                    << body_state.velocity.x << ","
                    << body_state.velocity.y << ","
                    << body_state.velocity.z << std::endl;
    }
    counter++;
}
