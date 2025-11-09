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
    PropertiesReader();
    static PropertiesReader& get_instance();

public:
    template<class... Keys>
    static std::string get_property(Keys&&... keys);

    template <class ... Keys>
    static std::vector<std::string> get_property_array(char delimiter, Keys&&... keys);

    template <class ... Keys>
    static std::string get_property_path(Keys&&... keys);
};

template <class ... Keys>
std::string PropertiesReader::get_property(Keys&&... keys)
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

template <class ... Keys>
std::vector<std::string> PropertiesReader::get_property_array(const char delimiter, Keys&&... keys)
{
    auto value = get_property(std::forward<Keys>(keys)...);
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(value);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;

}

template <class ... Keys>
std::string PropertiesReader::get_property_path(Keys&&... keys)
{
    const std::string ss = get_property(std::forward<Keys>(keys)...);
    return PathResolver::get_data(ss);
}


#endif //MESSENGERORBIT_PROPERTIESREADER_H
