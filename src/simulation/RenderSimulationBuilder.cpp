//
// Created by magofrays on 11/4/25.
//

#include "RenderSimulationBuilder.h"

#include "SimulationTime.h"
#include "../render/RenderSystem.h"
#include "../space/NewtonFormula.h"
#include "../space/SpaceObjectEntity.h"
#include "../utils/Constants.h"
#include "../utils/PropertiesReader.h"
#include "../utils/TimeConverter.h"
#include "../space/NewtonFormulaProxy.h"


RenderSimulationBuilder::RenderSimulationBuilder()
{
    start_date = PropertiesReader::get_property("forward-task", "simulation", "start-date");
    step = PropertiesReader::get_property("forward-task", "simulation", "step");
    end_date = PropertiesReader::get_property("forward-task", "simulation", "end-date");
}

Simulation* RenderSimulationBuilder::buildSimulation()
{
    simulation = new Simulation();
    simulation->set_start_date(TimeConverter::from_string_to_tdb(start_date));
    simulation->set_current_date(simulation->get_start_date());
    simulation->set_step(TimeConverter::from_string_to_tdb(step));
    simulation->set_end_date(TimeConverter::from_string_to_tdb(end_date));
    simulation_bodies.push_back(new SpaceObjectEntity("SUN"));
    simulation_bodies.push_back(new SpaceObjectEntity("MERCURY BARYCENTER"));
    simulation_bodies.push_back(new SpaceObjectEntity("VENUS BARYCENTER"));
    simulation_bodies.push_back(new SpaceObjectEntity("EARTH BARYCENTER"));
    simulation_bodies.push_back(new SpaceObjectEntity("MARS BARYCENTER"));
    simulation_bodies.push_back(new SpaceObjectEntity("JUPITER BARYCENTER"));
    simulation_bodies.push_back(new SpaceObjectEntity("SATURN BARYCENTER"));
    simulation_bodies.push_back(new SpaceObjectEntity("URANUS BARYCENTER"));
    simulation_bodies.push_back(new SpaceObjectEntity("NEPTUNE BARYCENTER"));

    auto parts = PropertiesReader::get_property_array(' ', "forward-task", "simulation", "messenger-color");

    ExtendedBodyState start_state_messenger;
    start_state_messenger.time = simulation->get_start_date();
    // Vec3d merc_pos = SpaceObjectManager::get_body_state_at_time(time, "MERCURY BARYCENTER").position;
    // start_state_messenger.position = Vec3d( 6.383826027878831E-03,  2.725647594289967E-01,  1.444990253752345E-01); // sun
    // start_state_messenger.velocity = Vec3d(-3.437382718770443E-02,  1.087859065804843E-03,  2.261564357222589E-03); // sun

    start_state_messenger.position = Vec3d(-2.5132478151856E-05,3.37750476154645E-06,-7.27851557240146E-06); // merc
    start_state_messenger.velocity = Vec3d(-0.000494791908675177,-0.00013503267241599,-0.00165501708001384); // merc

    NewtonFormulaProxy *messenger2 = new NewtonFormulaProxy(
        simulation_bodies,
        "MESSENGER",
        start_state_messenger,
        simulation->get_step(),
        "simulation_results.txt"
    );
    messenger2->set_use_implicit(true);

    Vec3d messenger_color = Vec3d(std::stoi(parts.at(0)), std::stoi(parts.at(1)), std::stoi(parts.at(2)))/255;
    messenger2->set_color(messenger_color);
    simulation_bodies.push_back(messenger2);


    for (auto & simulation_body : simulation_bodies)
    {
        RenderSystem::get_instance().register_object(simulation_body);
    }
    for (auto body : simulation_bodies)
    {
        std::cout << body->get_object_name() << std::endl;
        std::cout << body->get_body_state(simulation->get_start_date()) << std::endl;
    }
    simulation->set_space_objects(simulation_bodies);
    return simulation;
};



RenderSimulationBuilder::~RenderSimulationBuilder()
{
    // delete simulation;
    // for (const auto body : simulation_bodies)
    // {
    //     delete body;
    // }
}
