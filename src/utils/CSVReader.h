//
// Created by sun on 08.12.2025.
//

#ifndef MESSENGERORBIT_CSV_READER_H
#define MESSENGERORBIT_CSV_READER_H

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <map>

#include "PathResolver.h"
#include "../types/ExtendedBodyState.h"

struct ObservationData {
    double time_tag_seconds;
    std::string record_time;
    double full_observable;
    std::string receiving_station_id;
    double full_ref_freq;
    double Tc;
};

class CSVReader
{
    CSVReader();

public:

    static std::vector<ObservationData> read_csv(std::string& file_name, double start_time, double end_time) ;
    static ObservationData read_line(std::string& line, double start_time, double end_time);
    static std::map<SpiceDouble, ExtendedBodyState> read_csv_time(std::string file_name);
    static BodyState read_line_add(std::string& line);
};


#endif //MESSENGERORBIT_CSV_READER_H