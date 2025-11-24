//
// Created by magofrays on 10/30/25.
//

#ifndef MESSENGERORBIT_PATHRESOLVER_H
#define MESSENGERORBIT_PATHRESOLVER_H

#include <filesystem>
#include <string>


class PathResolver
{

    std::filesystem::path program_path;
    PathResolver()
    {
        program_path = std::filesystem::canonical("/proc/self/exe").parent_path();
    }
    static PathResolver& get_instance() {
        static PathResolver instance;
        return instance;
    }

    public:


    [[nodiscard]] static std::string get_program_path()
    {
        return get_instance().program_path.string();
    }

    [[nodiscard]] static std::string get_data(const std::string& path)
    {
        return ( get_instance().program_path / "../data"/ path).lexically_normal().string();
    }



};


#endif //MESSENGERORBIT_PATHRESOLVER_H