//
// Created by magofrays on 11/5/25.
//

#include "RenderFunctions.h"

#include <stdexcept>

#include "../types/Vec3d.h"
#include "../utils/PropertiesReader.h"

RenderFunctions::RenderFunctions()
{
    width = std::stoi(PropertiesReader::get_property("render", "width"));
    height = std::stoi(PropertiesReader::get_property("render", "height"));
    auto parts = PropertiesReader::get_property_array(' ', "render", "background");
    background_color = Vec3d(std::stoi(parts.at(0)), std::stoi(parts.at(1)), std::stoi(parts.at(2)))/255;
}

RenderFunctions& RenderFunctions::get_instance()
{
    static RenderFunctions instance;
    return instance;
}

void RenderFunctions::init()
{
    auto& instance = get_instance();
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    instance.window = glfwCreateWindow(
        instance.width, instance.height,
        "OpenGL Window", nullptr, nullptr);
    if (!instance.window)
    {
        throw std::runtime_error("Failed to initialize GLFW window");
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTranslatef(0.0f, 0.0f, -5.0f);
    glfwMakeContextCurrent(instance.window);
}

void RenderFunctions::pre_render()
{
    auto& instance = get_instance();
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(instance.background_color.x, instance.background_color.y, instance.background_color.z, 1.0f);
}

void RenderFunctions::set_perspective(float fov_degrees, float aspect, float near, float far) {
    float fov_rad = fov_degrees * M_PI / 180.0f;
    float f = 1.0f / tan(fov_rad / 2.0f);

    float matrix[16] = {
        f/aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (far+near)/(near-far), -1,
        0, 0, (2*far*near)/(near-far), 0
    };

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMultMatrixf(matrix);
    glMatrixMode(GL_MODELVIEW);
}

void RenderFunctions::post_render()
{
    auto& instance = get_instance();
    glfwSwapBuffers(instance.window);
    glfwPollEvents();
}

void RenderFunctions::draw_circle(Vec3d pos, Vec3d color, float radius)
{
    auto& instance = get_instance();

    glColor3f(color.x, color.y, color.z);

    int stacks = 16;
    int slices = 32;
    for (int i = 0; i <= stacks; ++i) {
        float phi = M_PI * i / stacks;
        glBegin(GL_LINE_LOOP);
        for (int j = 0; j <= slices; ++j) {
            float theta = 2.0f * M_PI * j / slices;

            float x1 = pos.x + radius * sin(phi) * cos(theta);
            float y1 = pos.y + radius * sin(phi) * sin(theta);
            float z1 = pos.z + radius * cos(phi);

            float x2 = pos.x + radius * sin(phi + M_PI/stacks) * cos(theta);
            float y2 = pos.y + radius * sin(phi + M_PI/stacks) * sin(theta);
            float z2 = pos.z + radius * cos(phi + M_PI/stacks);

            glVertex3f(x1, y1, z1);
            glVertex3f(x2, y2, z2);
        }
        glEnd();
    }
    glEnd();
}

void RenderFunctions::rotate(const Vec3d& rotate)
{
    glRotatef(rotate.x, 1, 0, 0);
    glRotatef(rotate.y, 0, 1, 0);
    glRotatef(rotate.z, 0, 0, 1);
}



GLFWwindow* RenderFunctions::get_window()
{
    auto& instance = get_instance();
    return instance.window;
}



