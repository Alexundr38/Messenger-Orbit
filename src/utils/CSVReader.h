//
// Created by sun on 08.12.2025.
//

#ifndef MESSENGERORBIT_CSV_READER_H
#define MESSENGERORBIT_CSV_READER_H

#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#include "PathResolver.h"

struct ObservationData {
    double time_tag_seconds;
    std::string record_time;
    double full_observable;
    std::string receiving_station_id;
    double full_ref_freq;
};

class CSVReader
{
    CSVReader();

public:

    static std::vector<ObservationData> read_csv(std::string& file_name);
    static ObservationData read_line(std::string& line);
};


#endif //MESSENGERORBIT_CSV_READER_H