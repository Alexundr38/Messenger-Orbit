//
// Created by magofrays on 12/12/25.
//

#include "MessengerSimulationBuilder.h"
#include "../utils/TimeConverter.h"
#include "../space/NewtonFormula.h"
#include "../space/NewtonFormulaProxy.h"
#include "../space/SpaceObjectEntity.h"

Simulation* MessengerSimulationBuilder::buildSimulation()
{
    SpiceDouble start_date = TimeConverter::from_string_to_tdb(PropertiesReader::get_property("forward-task", "simulation", "start-date"));
    SpiceDouble step = TimeConverter::from_string_to_tdb(PropertiesReader::get_property("forward-task", "simulation", "step"));
    SpiceDouble end_date = TimeConverter::from_string_to_tdb(PropertiesReader::get_property("forward-task", "simulation", "end-date"));
    BodyState start_state(
        Vec3d( 6.383826027878831E-03,  2.725647594289967E-01,  1.444990253752345E-01),
        Vec3d(-3.437382718770443E-02,  1.087859065804843E-03,  2.261564357222589E-03),
            start_date
        );
    auto newton = new NewtonFormulaProxy( "MESSENGER", start_state, step, "result.txt");
    auto* simulation = new Simulation();
    newton->add_force_body(new SpaceObjectEntity("SUN"));
    newton->add_force_body(new SpaceObjectEntity("MERCURY BARYCENTER"));
    newton->add_force_body(new SpaceObjectEntity("VENUS BARYCENTER"));
    newton->add_force_body(new SpaceObjectEntity("EARTH BARYCENTER"));
    newton->add_force_body(new SpaceObjectEntity("MARS BARYCENTER"));
    newton->add_force_body(new SpaceObjectEntity("JUPITER BARYCENTER"));
    newton->add_force_body(new SpaceObjectEntity("SATURN BARYCENTER"));
    newton->add_force_body(new SpaceObjectEntity("URANUS BARYCENTER"));
    newton->add_force_body(new SpaceObjectEntity("NEPTUNE BARYCENTER"));
    simulation->set_start_date(start_date);
    simulation->set_current_date(start_date);
    simulation->set_end_date(end_date);
    simulation->set_step(step);
    simulation->add_space_object(newton);
    return simulation;
}
