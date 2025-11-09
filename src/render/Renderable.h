//
// Created by magofrays on 11/4/25.
//

#ifndef MESSENGERORBIT_RENDERER_H
#define MESSENGERORBIT_RENDERER_H

#include <atomic>
#include <mutex>

#include "../types/BodyState.h"

class Renderable
{
protected:
    BodyState current_body_state;
    std::mutex state_mutex;

public:
    virtual ~Renderable() = default;

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