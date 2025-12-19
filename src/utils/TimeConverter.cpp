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
    SpiceDouble et_j2000, et_j1950;

    str2et_c("2000 JAN 01 12:00:00 TDB", &et_j2000);

    str2et_c("1950 JAN 01 00:00:00 TDB", &et_j1950);

    SpiceDouble j2000_to_j1950 = et_j2000 - et_j1950;

    SpiceDouble et;
    str2et_c(tdb.c_str(), &et);
    return et + j2000_to_j1950;
}

std::string TimeConverter::to_utc(const SpiceDouble tdb)
{
    std::lock_guard<std::mutex> lock(spice_mutex);
    SpiceDouble et_j2000, et_j1950;
    str2et_c("2000 JAN 01 12:00:00 TDB", &et_j2000);
    str2et_c("1950 JAN 01 00:00:00 TDB", &et_j1950);
    SpiceDouble j2000_to_j1950 = et_j2000 - et_j1950;
    get_instance();
    char buffer[256];
    et2utc_c(tdb-j2000_to_j1950, "C", 3, sizeof(buffer), buffer);
    return std::string(buffer);
}


