//
// Created by magofrays on 11/4/25.
//

#ifndef MESSENGERORBIT_RENDERSYSTEM_H
#define MESSENGERORBIT_RENDERSYSTEM_H
#include <vector>
#include "Camera.h"
#include "Renderable.h"

class RenderSystem
{
    std::vector<Renderable*> render_objects;
    Camera * camera = nullptr;
    double proportion;
    RenderSystem();
public:
    static RenderSystem& get_instance();
    void register_object(Renderable* obj);
    void set_camera(Camera* camera);
    void unregister_object(const Renderable* obj);
    void draw_path(const std::shared_ptr<const std::vector<Vec3d>>& history);
    void render();
};


#endif //MESSENGERORBIT_RENDERSYSTEM_H