//
// Created by magofrays on 11/4/25.
//

#include "MessengerSimulationBuilder.h"

#include "../render/RenderSystem.h"
#include "../space/NewtonFormula.h"
#include "../space/SpaceObjectEntity.h"
#include "../utils/PropertiesReader.h"
#include "../utils/TimeConverter.h"
#include "../utils/SpaceObjectManager.h"

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
    simulation_bodies.push_back(new SpaceObjectEntity("EARTH BARYCENTER"));
    simulation_bodies.push_back(new SpaceObjectEntity("MARS BARYCENTER"));
    simulation_bodies.push_back(new SpaceObjectEntity("JUPITER BARYCENTER"));
    simulation_bodies.push_back(new SpaceObjectEntity("SATURN BARYCENTER"));
    simulation_bodies.push_back(new SpaceObjectEntity("URANUS BARYCENTER"));
    simulation_bodies.push_back(new SpaceObjectEntity("NEPTUNE BARYCENTER"));

    auto parts = PropertiesReader::get_property_array(' ', "forward-task", "simulation", "messenger-color");


    SpiceDouble time = TimeConverter::to_tdb(start_date);
    BodyState start_state_messenger;
    BodyState sun_to_mercury = SpaceObjectManager::get_body_state_at_time(time, "MERCURY BARYCENTER", "SOLAR SYSTEM BARYCENTER");
    start_state_messenger.time = time;
    start_state_messenger.position = Vec3d(1.720997744411687E2, -1.069890504504989e3, 2.669916542738690E3);
    start_state_messenger.velocity = Vec3d(-3.500275520598676, 2.871081348963887E-1, 3.302986684169092E-1);
    // start_state_messenger.position -= sun_to_mercury.position;
    // start_state_messenger.velocity -= sun_to_mercury.velocity;
    // start_state_messenger.position += Vec3d(2639.0, 0.0, 0.0);
    // start_state_messenger.velocity += Vec3d(0.0, 2.889, 0.0);

    NewtonFormula *messenger = new NewtonFormula(
        simulation_bodies,
        "MESSENGER",
        start_state_messenger,
        simulation->step
    );
    // NewtonFormula *messenger2 = new NewtonFormula(
    //     simulation_bodies,
    //     "MESSENGER",
    //     start_state_messenger,
    //     simulation->step
    // );

    messenger->set_newton_methods(true);
    messenger->set_stiffness_threshold(0.1);
    // messenger2->set_implicit_methods(false);
    // messenger2->set_use_corrector(true);
    // messenger2->set_newton_methods(false);
    Vec3d messenger_color = Vec3d(std::stoi(parts.at(0)), std::stoi(parts.at(1)), std::stoi(parts.at(2)))/255;
    messenger->set_color(messenger_color);
    // messenger2->set_color(messenger_color);
    simulation_bodies.push_back(messenger);
    // simulation_bodies.push_back(messenger2);


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
