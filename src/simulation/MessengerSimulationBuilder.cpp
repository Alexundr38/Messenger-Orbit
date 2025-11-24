//
// Created by magofrays on 11/4/25.
//

#include "MessengerSimulationBuilder.h"

#include "../render/RenderSystem.h"
#include "../space/NewtonFormula.h"
#include "../space/SpaceObjectEntity.h"
#include "../utils/PropertiesReader.h"
#include "../utils/TimeConverter.h"

MessengerSimulationBuilder::MessengerSimulationBuilder()
{
    start_date = PropertiesReader::get_property("forward-task", "simulation", "start-date");
    step = PropertiesReader::get_property("forward-task", "simulation", "step");
    end_date = PropertiesReader::get_property("forward-task", "simulation", "end-date");
}

Simulation* MessengerSimulationBuilder::buildSimulation()
{
    simulation = new Simulation();
    simulation->start_date = TimeConverter::to_tdb(start_date);
    simulation->current_date = simulation->start_date;
    simulation->step = TimeConverter::to_tdb(step);
    simulation->end_date = TimeConverter::to_tdb(end_date);
    simulation_bodies.push_back(new SpaceObjectEntity("SUN"));
    simulation_bodies.push_back(new SpaceObjectEntity("MERCURY BARYCENTER"));
    simulation_bodies.push_back(new SpaceObjectEntity("VENUS BARYCENTER"));
    simulation_bodies.push_back(new SpaceObjectEntity("EARTH"));
    simulation_bodies.push_back(new SpaceObjectEntity("MARS BARYCENTER"));
    simulation_bodies.push_back(new SpaceObjectEntity("JUPITER BARYCENTER"));
    simulation_bodies.push_back(new SpaceObjectEntity("SATURN BARYCENTER"));
    simulation_bodies.push_back(new SpaceObjectEntity("URANUS BARYCENTER"));
    simulation_bodies.push_back(new SpaceObjectEntity("NEPTUNE BARYCENTER"));
    for (auto body : simulation_bodies)
    {
        std::cout << body->get_object_name() << std::endl;
        std::cout << body->get_body_state(simulation->start_date) << std::endl;
    }
    auto parts = PropertiesReader::get_property_array(' ', "forward-task", "simulation", "messenger-color");


    BodyState start_state_messenger;
    start_state_messenger.time = TimeConverter::to_tdb(start_date);
    start_state_messenger.position = Vec3d(9.550067806899118e5, 4.600907778618815e7, 3.613570817987552e6);
    start_state_messenger.velocity = Vec3d(-5.951679809132351e1, 3.285768007622105, 2.843431738797426);
    SpaceObject *messenger = new NewtonFormula(
        simulation_bodies,
        "MESSENGER",
        start_state_messenger,
        simulation->step
    );
    Vec3d messenger_color = Vec3d(std::stoi(parts.at(0)), std::stoi(parts.at(1)), std::stoi(parts.at(2)))/255;
    messenger->set_color(messenger_color);
    simulation_bodies.push_back(messenger);


    for (size_t i = 0; i < simulation_bodies.size(); ++i)
    {
        RenderSystem::get_instance().register_object(simulation_bodies[i]);
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
