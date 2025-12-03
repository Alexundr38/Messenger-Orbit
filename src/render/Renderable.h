//
// Created by magofrays on 11/4/25.
//

#ifndef MESSENGERORBIT_RENDERER_H
#define MESSENGERORBIT_RENDERER_H


#include <mutex>
#include "../utils/PropertiesReader.h"
#include "../types/BodyState.h"
#include "../utils/Constants.h"

class Renderable
{
protected:
    std::vector<Vec3d> history;
    BodyState current_body_state;
    Vec3d color;
    std::vector<Vec3d> history_private;
    mutable std::mutex history_mutex;
    std::shared_ptr<const std::vector<Vec3d>> render_snapshot;

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
        color = Vec3d(std::stoi(parts.at(0)), std::stoi(parts.at(1)), std::stoi(parts.at(2)))/color_max;
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

    void add_history_point(const Vec3d& point) {
        std::lock_guard lock(history_mutex);
        history_private.push_back(point);

        if (history_private.size() % 100 == 0) {
            auto new_snapshot = std::make_shared<std::vector<Vec3d>>(history_private);
            render_snapshot = new_snapshot;
        }
    }

    std::shared_ptr<const std::vector<Vec3d>> get_render_snapshot() const {
        std::lock_guard lock(history_mutex);
        return render_snapshot;
    }
};


#endif //MESSENGERORBIT_RENDERER_H