//
// Created by magofrays on 11/4/25.
//

#include "MessengerSimulationBuilder.h"

#include "../render/RenderSystem.h"
#include "../space/NewtonFormula.h"
#include "../space/SpaceObjectEntity.h"
#include "../utils/Constants.h"
#include "../utils/PropertiesReader.h"
#include "../utils/TimeConverter.h"
#include "../space/NewtonFormulaProxy.h"


MessengerSimulationBuilder::MessengerSimulationBuilder()
{
    start_date = PropertiesReader::get_property("forward-task", "simulation", "start-date");
    step = PropertiesReader::get_property("forward-task", "simulation", "step");
    end_date = PropertiesReader::get_property("forward-task", "simulation", "end-date");
}

Simulation* MessengerSimulationBuilder::buildSimulation()
{
    simulation = new Simulation();
    auto j2000 = TimeConverter::to_tdb(PropertiesReader::get_property("forward-task", "simulation", "j2000"));
    simulation->start_date = TimeConverter::to_tdb(start_date)+j2000;
    simulation->current_date = simulation->start_date;
    simulation->step = TimeConverter::to_tdb(step);
    simulation->end_date = TimeConverter::to_tdb(end_date);
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


    SpiceDouble time = TimeConverter::to_tdb(start_date) - j2000;
    BodyState start_state_messenger;
    start_state_messenger.time = time/day;
    start_state_messenger.position = Vec3d(1.150415935974740E-6, -7.151776288651341E-06,  1.784728973912254E-05);
    start_state_messenger.velocity = Vec3d(-2.021578272234898E-03,  1.658188230820052E-04,  1.907634434747403E-04);

    NewtonFormulaProxy *messenger2 = new NewtonFormulaProxy(
        simulation_bodies,
        "MESSENGER",
        start_state_messenger,
        simulation->step/day
    );

    messenger2->set_implicit_methods(false);
    messenger2->set_use_corrector(true);
    messenger2->set_newton_methods(false);
    messenger2->set_use_relativistic_corrections(true);
    Vec3d messenger_color = Vec3d(std::stoi(parts.at(0)), std::stoi(parts.at(1)), std::stoi(parts.at(2)))/255;
    messenger2->set_color(messenger_color);
    simulation_bodies.push_back(messenger2);


    for (size_t i = 0; i < simulation_bodies.size(); ++i)
    {
        RenderSystem::get_instance().register_object(simulation_bodies[i]);

    }
    for (auto body : simulation_bodies)
    {
        std::cout << body->get_object_name() << std::endl;
        std::cout << body->get_body_state(simulation->start_date) << std::endl;
    }
    simulation->spaceObjects = simulation_bodies;
    return simulation;
};



MessengerSimulationBuilder::~MessengerSimulationBuilder()
{
    // delete simulation;
    // for (const auto body : simulation_bodies)
    // {
    //     delete body;
    // }
}
