//
// Created by magofrays on 12/19/25.
//

#include "BoostSimulationBuilder.h"
#include "../utils/TimeConverter.h"
#include "../space/SpaceObjectEntity.h"
#include "BoostSimulation.h"

Simulation* BoostSimulationBuilder::buildSimulation()
{
    SpiceDouble start_date = TimeConverter::from_string_to_tdb(PropertiesReader::get_property("forward-task", "simulation", "start-date"));
    SpiceDouble step = TimeConverter::from_string_to_tdb(PropertiesReader::get_property("forward-task", "simulation", "step"));
    SpiceDouble end_date = TimeConverter::from_string_to_tdb(PropertiesReader::get_property("forward-task", "simulation", "end-date"));
    std::cout << std::fixed << std::setprecision(0) << start_date << " " << step << " " << end_date << std::endl;
    ExtendedBodyState start_state(
        Vec3d( 6.383826027878831E-03,  2.725647594289967E-01,  1.444990253752345E-01),
        Vec3d(-3.437382718770443E-02,  1.087859065804843E-03,  2.261564357222589E-03),
            start_date
        );
    // ExtendedBodyState start_state(
    //     Vec3d( -2.513247815185603E-05,  3.377504761546451E-06, -7.278515572401461E-06),
    //     Vec3d(-4.947919086751768E-04, -1.350326724159897E-04, -1.655017080013835E-03),
    //     start_date
    // );
    NewtonFormula *newton = new NewtonFormulaProxy( "MESSENGER", start_state, step, "result.txt");
    newton->set_use_implicit(false);
    newton->add_force_body(new SpaceObjectEntity("SUN"));
    newton->add_force_body(new SpaceObjectEntity("MERCURY BARYCENTER"));
    newton->add_force_body(new SpaceObjectEntity("VENUS BARYCENTER"));
    newton->add_force_body(new SpaceObjectEntity("EARTH BARYCENTER"));
    newton->add_force_body(new SpaceObjectEntity("MARS BARYCENTER"));
    newton->add_force_body(new SpaceObjectEntity("JUPITER BARYCENTER"));
    newton->add_force_body(new SpaceObjectEntity("SATURN BARYCENTER"));
    newton->add_force_body(new SpaceObjectEntity("URANUS BARYCENTER"));
    newton->add_force_body(new SpaceObjectEntity("NEPTUNE BARYCENTER"));
    auto* simulation = new BoostSimulation(newton);
    simulation->set_start_date(start_date/day);
    simulation->set_current_date(start_date/day);
    simulation->set_end_date(end_date/day);
    simulation->set_step((long double)step/day);
    // simulation->add_space_object(newton);
    return simulation;
}
