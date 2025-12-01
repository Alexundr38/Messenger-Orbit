//
// Created by magofrays on 10/30/25.
//
#include <thread>

#include "render/CameraController.h"
#include "render/RenderFunctions.h"
#include <glm/gtc/type_ptr.hpp>

#include "render/RenderSystem.h"
#include "simulation/MessengerSimulationBuilder.h"
#include "simulation/SimulationBuilder.h"

#include <atomic>
#include <iostream>


int main(int argc, char* argv[])
{
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    RenderSystem::get_instance().set_camera(&camera);
    CameraController::init(&camera);
    std::atomic<bool> simulation_running{true};
    Simulation *simulation = MessengerSimulationBuilder().buildSimulation();
    std::thread simulation_thread([simulation, &simulation_running]() {
        try {
            simulation->run(simulation_running);
        } catch (const std::exception& e) {
            std::cerr << "Simulation error: " << e.what() << std::endl;
        }
    });

    while (!glfwWindowShouldClose(RenderFunctions::get_window()))
    {
        CameraController::update();
        RenderSystem::get_instance().render();
        // std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
    simulation_running = false;
    if (simulation_thread.joinable()) {
        simulation_thread.join();
    }

    delete simulation;
    return 0;
}