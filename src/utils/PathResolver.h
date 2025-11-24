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
    PathResolver();
    static PathResolver& get_instance();

public:
    [[nodiscard]] static std::string get_program_path();
    [[nodiscard]] static std::string get_data(const std::string& path);
};


#endif //MESSENGERORBIT_PATHRESOLVER_H