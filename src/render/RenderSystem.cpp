//
// Created by magofrays on 11/4/25.
//

#include "RenderSystem.h"
#include <algorithm>
#include <GLFW/glfw3.h>

#include "RenderFunctions.h"
#include "../space/SpaceObject.h"
#include "../utils/Constants.h"
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

void RenderSystem::render()
{
    RenderFunctions::pre_render();
    {
        glLoadMatrixf(glm::value_ptr(camera->GetViewMatrix()));
        SpiceDouble simulation_time_tdb;
        for (auto render_object : render_objects)
        {
            simulation_time_tdb = render_object->get_current_body_state().time;
            auto pos = render_object->get_current_body_state().position/proportion;
            RenderFunctions::draw_sphere_wireframe(pos, render_object->get_color(), render_object->get_size()/au/proportion);

        }
        std::string utc_string = TimeConverter::to_utc(simulation_time_tdb);
        std::ostringstream oss;
        oss << utc_string;
        glfwSetWindowTitle(RenderFunctions::get_window(), oss.str().c_str());

    }
    RenderFunctions::post_render();
}

