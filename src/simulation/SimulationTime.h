//
// Created by magofrays on 12/10/25.
//

#ifndef MESSENGERORBIT_SIMMULATIONTIME_H
#define MESSENGERORBIT_SIMMULATIONTIME_H
#include <mutex>
#include <SpiceZdf.h>


class SimulationTime
{
    SpiceDouble current_time;
    mutable std::mutex time_mutex;

    explicit SimulationTime() : current_time(0)
    {
    }

public:
    static SimulationTime& get_instance()
    {
        static SimulationTime instance;
        return instance;
    }

    static SpiceDouble get()
    {
        auto& instance = get_instance();
        std::lock_guard<std::mutex> lock(instance.time_mutex);
        return instance.current_time;
    }

    static void set(SpiceDouble new_time)
    {
        auto& instance = get_instance();
        std::lock_guard<std::mutex> lock(instance.time_mutex);
        instance.current_time = new_time;
    }

    static void add(SpiceDouble delta)
    {
        auto& instance = get_instance();
        std::lock_guard<std::mutex> lock(instance.time_mutex);
        instance.current_time += delta;
    }
};


#endif //MESSENGERORBIT_SIMMULATIONTIME_H