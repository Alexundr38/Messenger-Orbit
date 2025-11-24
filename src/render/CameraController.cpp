#include "CameraController.h"

#include "RenderFunctions.h"

void CameraController::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    auto& instance = get_instance();
    
    if (instance.firstMouse)
    {
        instance.lastX = xpos;
        instance.lastY = ypos;
        instance.firstMouse = false;
    }

    float xoffset = xpos - instance.lastX;
    float yoffset = instance.lastY - ypos; // обратный порядок, так как y-координаты идут снизу вверх

    instance.lastX = xpos;
    instance.lastY = ypos;

    if (instance.camera) {
        instance.camera->ProcessMouseMovement(xoffset, yoffset);
    }
    
    // Возвращаем курсор в центр
    glfwSetCursorPos(window, instance.center_x, instance.center_y);
    instance.lastX = instance.center_x;
    instance.lastY = instance.center_y;
}

void CameraController::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto& instance = get_instance();
    
    // Сохраняем состояние клавиш для непрерывного движения
    static bool keys[1024] = {false};
    
    if (action == GLFW_PRESS)
        keys[key] = true;
    else if (action == GLFW_RELEASE)
        keys[key] = false;
    
    // Обрабатываем движение в функции update()
}

void CameraController::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    auto& instance = get_instance();
    if (instance.camera) {
        instance.camera->ProcessMouseScroll(yoffset);
    }
}

void CameraController::init(Camera* camera)
{
    auto& instance = get_instance();
    instance.camera = camera;
    instance.window = RenderFunctions::get_window();
    
    int width, height;
    glfwGetWindowSize(instance.window, &width, &height);
    instance.center_x = width / 2.0;
    instance.center_y = height / 2.0;

    instance.lastX = instance.center_x;
    instance.lastY = instance.center_y;
    
    // Регистрируем callback'и
    glfwSetCursorPosCallback(instance.window, mouse_callback);
    glfwSetKeyCallback(instance.window, key_callback);
    glfwSetScrollCallback(instance.window, scroll_callback);
    
    // Захватываем курсор
    glfwSetInputMode(instance.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    instance.lastFrameTime = glfwGetTime();
}

void CameraController::update()
{
    auto& instance = get_instance();
    
    // Вычисляем deltaTime
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - instance.lastFrameTime);
    instance.lastFrameTime = currentTime;
    
    if (!instance.camera) return;
    
    // Обрабатываем непрерывное движение
    if (glfwGetKey(instance.window, GLFW_KEY_W) == GLFW_PRESS)
        instance.camera->ProcessKeyboard(0, deltaTime); // FORWARD
    if (glfwGetKey(instance.window, GLFW_KEY_S) == GLFW_PRESS)
        instance.camera->ProcessKeyboard(1, deltaTime); // BACKWARD
    if (glfwGetKey(instance.window, GLFW_KEY_A) == GLFW_PRESS)
        instance.camera->ProcessKeyboard(2, deltaTime); // LEFT
    if (glfwGetKey(instance.window, GLFW_KEY_D) == GLFW_PRESS)
        instance.camera->ProcessKeyboard(3, deltaTime); // RIGHT
    if (glfwGetKey(instance.window, GLFW_KEY_SPACE) == GLFW_PRESS)
        instance.camera->ProcessVerticalMovement(0, deltaTime); // UP
    if (glfwGetKey(instance.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        instance.camera->ProcessVerticalMovement(1, deltaTime); // DOWN
}