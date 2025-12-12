#ifndef MESSENGERORBIT_RENDERER_H
#define MESSENGERORBIT_RENDERER_H

#include "../utils/PropertiesReader.h"
#include "../types/BodyState.h"
#include "../utils/Constants.h"

class Renderable
{
protected:
    std::vector<Vec3d> history;
    BodyState current_body_state;
    Vec3d color;
    float size = 1;

public:
    virtual ~Renderable() = default;

    Renderable()
    {
        auto parts = PropertiesReader::get_property_array(' ', "render", "planets-color");
        color = Vec3d(std::stoi(parts.at(0)), std::stoi(parts.at(1)), std::stoi(parts.at(2))) / color_max;
    }

    void set_size(float new_size)
    {
        size = new_size;
    }

    void set_color(const Vec3d& new_color)
    {
        color = new_color;
    }

    Vec3d get_color() const
    {
        return color;
    }

    float get_size() const
    {
        return size;
    }

    virtual BodyState get_current_body_state() const
    {
        return current_body_state;
    }

    virtual void set_current_body_state(const BodyState& body_state)
    {
        current_body_state = body_state;
    }

    void add_history_point(const Vec3d& point)
    {
        history.push_back(point);
    }

    const std::vector<Vec3d>& get_history() const
    {
        return history;
    }

    std::vector<Vec3d>& get_history_mutable()
    {
        return history;
    }

    void clear_history()
    {
        history.clear();
    }

    size_t get_history_size() const
    {
        return history.size();
    }

    bool is_history_empty() const
    {
        return history.empty();
    }
};

#endif