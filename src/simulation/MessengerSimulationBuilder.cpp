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
        std::cout << body->get_body_state(simulation->start_date) << std::endl;
    }

    BodyState start_state_messenger = SpaceObjectManager::get_body_state_at_time(simulation->start_date, "MERCURY");
    start_state_messenger.position += Vec3d(200, 0, 0);
    start_state_messenger.velocity += Vec3d(0.0, 2.89, 0.1);
    SpaceObject *messenger = new NewtonFormula(
        simulation_bodies,
        "MESSENGER",
        start_state_messenger,
        simulation->step
    );
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
