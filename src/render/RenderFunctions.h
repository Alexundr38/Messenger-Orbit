//
// Created by magofrays on 11/5/25.
//

#ifndef MESSENGERORBIT_RENDERFUNCTIONS_H
#define MESSENGERORBIT_RENDERFUNCTIONS_H
#include "GLFW/glfw3.h"
#include "../types/Vec3d.h"

class RenderFunctions
{
    GLFWwindow* window = nullptr;
    unsigned int width, height;
    Vec3d background_color;
    RenderFunctions();
    static RenderFunctions& get_instance();
public:
    static void init();
    static void pre_render();
    static void set_perspective(float fov_degrees, float aspect, float near, float far);
    static void post_render();
    static void draw_circle(Vec3d pos, Vec3d color, float radius);
    static void rotate(const Vec3d& rotate);


    static GLFWwindow * get_window();
};


#endif //MESSENGERORBIT_RENDERFUNCTIONS_H