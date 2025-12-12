//
// Created by magofrays on 10/31/25.
//

#include "TimeConverter.h"
#include <SpiceUsr.h>
#include "SpiceGuard.h"
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

SpiceDouble TimeConverter::from_string_to_tdb(const std::string& tdb)
{
    get_instance();
    SpiceDouble et;
    str2et_c(tdb.c_str(), &et);
    return et;
}

std::string TimeConverter::to_utc(const SpiceDouble tdb)
{
    std::lock_guard<std::mutex> lock(spice_mutex);
    get_instance();
    char buffer[256];
    et2utc_c(tdb, "C", 3, sizeof(buffer), buffer);
    return std::string(buffer);
}


