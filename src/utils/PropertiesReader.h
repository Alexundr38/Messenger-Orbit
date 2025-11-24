//
// Created by magofrays on 10/30/25.
//

#ifndef MESSENGERORBIT_PROPERTIESREADER_H
#define MESSENGERORBIT_PROPERTIESREADER_H

#include <iostream>
#include <yaml-cpp/yaml.h>
#include "PathResolver.h"

class PropertiesReader
{
    YAML::Node properties;
    PropertiesReader() {
        properties = YAML::LoadFile(PathResolver::get_data("properties.yaml"));
    }
    static PropertiesReader& get_instance() {
        static PropertiesReader instance;
        return instance;
    }
    public:
    template<typename... Keys>
    static std::string get_property(Keys&&... keys)
    {
        auto& instance = get_instance();
        YAML::Node current = YAML::Clone(instance.properties);
        std::vector<std::string> keys_list = {std::string(keys)...};

        for (const auto& key : keys_list) {
            if (current && current[key]) {
                current = current[key];
            } else {
                throw std::runtime_error("Property not found");
            }
        }

        if (current) {
            return current.as<std::string>();
        }

        throw std::runtime_error("Property not found");
    }

    template <typename ... Keys>
    static std::string get_property_path(Keys&&... keys)
    {
        const std::string ss = get_property(std::forward<Keys>(keys)...);
        return PathResolver::get_data(ss);
    }

};




#endif //MESSENGERORBIT_PROPERTIESREADER_H
