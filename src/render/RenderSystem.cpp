//
// Created by magofrays on 11/4/25.
//

#include "RenderSystem.h"
#include <algorithm>
#include <GLFW/glfw3.h>

#include "RenderFunctions.h"
#include "../simulation/SimulationTime.h"
#include "../space/SpaceObject.h"
#include "../utils/Constants.h"
#include "../utils/SpaceObjectManager.h"
#include "glm/gtc/type_ptr.hpp"
#include "../utils/TimeConverter.h"

RenderSystem::RenderSystem()
{
    RenderFunctions::init();
    RenderFunctions::set_perspective(45.0f, 16.0f/9.0f, 0.1f, 100.0f);
    proportion = std::stod(PropertiesReader::get_property("render", "proportion"));
}

RenderSystem& RenderSystem::get_instance()
{
    static RenderSystem instance;
    return instance;
}

void RenderSystem::register_object(Renderable* obj)
{
    render_objects.push_back(obj);
}

void RenderSystem::set_camera(Camera * camera)
{
    this->camera = camera;
}

void RenderSystem::unregister_object(const Renderable* obj)
{
    auto it = std::find(render_objects.begin(), render_objects.end(), obj);
    if (it != render_objects.end()) render_objects.erase(it);
}

void RenderSystem::draw_path(const std::vector<Vec3d>& history, Vec3d center)
{
    for (int i = 0; i < history.size(); i++)
    {
        Vec3d point = history.at(i);//-center;
        RenderFunctions::draw_dot(point/proportion, Vec3d(1, 0, 1), 1);
    }
}

void RenderSystem::render()
{
    RenderFunctions::pre_render();
    {
        double simulation_time = render_objects[0]->get_current_body_state().time;
        center = SpaceObjectManager::get_body_state_at_time(simulation_time, "MERCURY BARYCENTER").position;
        // center += render_objects[render_objects.size()-1]->get_current_body_state().position;
        glLoadMatrixf(glm::value_ptr(camera->GetViewMatrix()));

        for (auto render_object : render_objects)
        {
            auto pos = (render_object->get_current_body_state().position - center)/proportion;
            if (dynamic_cast<SpaceObject*>(render_object)->get_object_name() == "MESSENGER")
            {
                pos = (render_object->get_current_body_state().position - center);
                pos += SpaceObjectManager::get_body_state_at_time(simulation_time, "MERCURY").position;
                pos /= proportion;
            }

            RenderFunctions::draw_sphere_wireframe(pos, render_object->get_color(), render_object->get_size()/au/proportion); //0.01f);
            auto history = render_object->get_history();
            draw_path(history, center);
        }
        std::string utc_string = TimeConverter::to_utc(simulation_time);
        std::ostringstream oss;
        oss << utc_string;
        glfwSetWindowTitle(RenderFunctions::get_window(), oss.str().c_str());

    }
    RenderFunctions::post_render();
}

