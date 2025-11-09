#ifndef MESSENGERORBIT_CONTROLLER_H
#define MESSENGERORBIT_CONTROLLER_H

#include <iostream>
#include <glm/glm.hpp>
#include "Camera.h"
#include "GLFW/glfw3.h"

class CameraController
{

    GLFWwindow* window;
    Camera* camera;
    double center_x = 0, center_y = 0;
    
    // Для управления камерой
    bool firstMouse = true;
    float lastX = 0.0f;
    float lastY = 0.0f;
    double lastFrameTime = 0.0;
    
    CameraController() = default;

    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    static CameraController& get_instance()
    {
        static CameraController instance;
        return instance;
    }

public:
    static void init(Camera* camera);
    static void update();
};

#endif