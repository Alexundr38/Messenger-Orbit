//
// Created by magofrays on 10/31/25.
//

#ifndef MESSENGERORBIT_TIMECONVERTER_H
#define MESSENGERORBIT_TIMECONVERTER_H
#include <SpiceZdf.h>
#include <string>


class TimeConverter
{
    TimeConverter();
    static TimeConverter& get_instance();
    public:
    static SpiceDouble to_tdb(const std::string& utc);
    static std::string to_utc(SpiceDouble tdb);
};


#endif //MESSENGERORBIT_TIMECONVERTER_H