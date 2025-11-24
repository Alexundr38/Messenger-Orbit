//
// Created by magofrays on 10/31/25.
//
#include "PropertiesReader.h"


PropertiesReader::PropertiesReader()
{
    properties = YAML::LoadFile(PathResolver::get_data("properties.yaml"));
}

PropertiesReader& PropertiesReader::get_instance()
{
    static PropertiesReader instance;
    return instance;
}
