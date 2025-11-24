//
// Created by magofrays on 10/31/25.
//

#include "TimeConverter.h"
#include <SpiceUsr.h>
#include "PropertiesReader.h"

TimeConverter::TimeConverter()
{
    furnsh_c(PropertiesReader::get_property_path("forward-task", "time-converter", "tls-path").c_str());
}

TimeConverter& TimeConverter::get_instance()
{
    static TimeConverter instance;
    return instance;
}

SpiceDouble TimeConverter::to_tdb(const std::string& utc)
{
    get_instance();
    SpiceDouble et;
    utc2et_c(utc.c_str(), &et);
    return et;
}

std::string TimeConverter::to_utc(const SpiceDouble et)
{
    char buffer[50];
    et2utc_c(et, "C", 3, sizeof(buffer), buffer);
    return std::string(buffer);
}


