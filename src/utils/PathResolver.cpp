//
// Created by magofrays on 11/1/25.
//
#include "PathResolver.h"

PathResolver::PathResolver()
{
    program_path = std::filesystem::canonical("/proc/self/exe").parent_path();
}

PathResolver& PathResolver::get_instance()
{
    static PathResolver instance;
    return instance;
}

std::string PathResolver::get_program_path()
{
    return get_instance().program_path.string();
}

std::string PathResolver::get_data(const std::string& path)
{
    return ( get_instance().program_path / "../data"/ path).lexically_normal().string();
}
