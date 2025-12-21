//
// Created by sun on 08.12.2025.
//

#include "CSVReader.h"
#include <iostream>
#include <algorithm>

#include "Constants.h"

std::vector<ObservationData> CSVReader::read_csv(std::string& file_name, double start_time, double end_time) {
    std::vector<ObservationData> observations;
    std::ifstream file(PathResolver::get_data_csv(file_name));

    std::string line;
    bool firstLine = true;

    while (std::getline(file, line)) {
        if (firstLine) {
            firstLine = false;
            continue;
        }

        if (line.empty()) {
            continue;
        }

        ObservationData data = read_line(line, start_time, end_time);
        if (data.time_tag_seconds != 0) {
            observations.push_back(data);
        }
    }

    std::sort(observations.begin(), observations.end(),
            [](const ObservationData& a, const ObservationData& b) {
                return a.time_tag_seconds < b.time_tag_seconds;
            });

    /*for (auto& observation : observations) {
        std::cout << observation.record_time << " " << observation.time_tag_seconds << " " << observation.full_observable << " " << observation.full_ref_freq
        << " " << observation.receiving_station_id << std::endl;
    }*/
    file.close();
    return observations;
}


ObservationData CSVReader::read_line(std::string& line, double start_time, double end_time) {
    ObservationData data;
    data.time_tag_seconds = 0;
    std::stringstream ss(line);
    std::string field;
    std::vector<std::string> fields;

    while (std::getline(ss, field, ',')) {
        fields.push_back(field);
    }

    if (std::stod(fields[0]) /day >= start_time && std::stod(fields[0]) /day <= end_time) {
        if (fields.size() == 14) {
            data.time_tag_seconds = std::stod(fields[0]) / day;
            data.record_time = fields[1];
            data.full_observable = std::stod(fields[2]);
            data.receiving_station_id = fields[5];
            data.full_ref_freq = std::stod(fields[8]);
        }
    }

    return data;
}
