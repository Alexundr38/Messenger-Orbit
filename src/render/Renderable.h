//
// Created by magofrays on 11/4/25.
//

#ifndef MESSENGERORBIT_RENDERER_H
#define MESSENGERORBIT_RENDERER_H


#include <mutex>
#include "../utils/PropertiesReader.h"
#include "../types/BodyState.h"

class Renderable
{
protected:
    std::vector<Vec3d> history;
    BodyState current_body_state;
    Vec3d color;

public:
    void set_size(float size)
    {
        this->size = size;
    }
    void set_color(const Vec3d& color)
    {
        this->color = color;
    }

    [[nodiscard]] Vec3d get_color() const
    {
        return color;
    }

    [[nodiscard]] float get_size() const
    {
        return size;
    }

protected:
    float size = 1;
    std::mutex state_mutex;

public:
    virtual ~Renderable() = default;
    Renderable()
    {
        auto parts = PropertiesReader::get_property_array(' ', "render", "planets-color");
        color = Vec3d(std::stoi(parts.at(0)), std::stoi(parts.at(1)), std::stoi(parts.at(2)))/255;
    }
    virtual BodyState get_current_body_state()
    {
        std::lock_guard<std::mutex> lock(state_mutex);
        return current_body_state;
    }

    virtual void set_current_body_state(const BodyState& body_state)
    {
        std::lock_guard<std::mutex> lock(state_mutex);
        current_body_state = body_state;
    }
};


#endif //MESSENGERORBIT_RENDERER_H