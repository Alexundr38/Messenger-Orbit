//
// Created by sun on 08.12.2025.
//

#include "CSVReader.h"
#include <iostream>
#include <algorithm>

#include "Constants.h"
#include <cmath>


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

    while (std::getline(ss, field, ' ')) {
        fields.push_back(field);
    }

    double current_time = (std::stod(fields[0]) + std::stod(fields[1]) * 0.001) /day ;
    if (current_time >= start_time && current_time <= end_time) {
        if (fields.size() == 12) {
            data.time_tag_seconds = current_time;
            //data.record_time = 0;
            data.full_observable = std::stod(fields[7]) + std::stod(fields[8]) * 1e-9;
            data.receiving_station_id = fields[6];
            data.full_ref_freq = (std::stod(fields[4]) * std::pow(2, 24) + std::stod(fields[5])) / 1000;
            data.Tc = (std::stod(fields[1]) * 0.001) / day;
        }
    }

    return data;
}


std::map<SpiceDouble, ExtendedBodyState> CSVReader::read_csv_time(std::string file_name) {
    std::map<SpiceDouble, ExtendedBodyState> states;
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

        BodyState data = read_line_add(line);
        ExtendedBodyState cur_data = ExtendedBodyState(data);
        states[data.time] = cur_data;
    }

    /*for (auto& observation : observations) {
        std::cout << observation.record_time << " " << observation.time_tag_seconds << " " << observation.full_observable << " " << observation.full_ref_freq
        << " " << observation.receiving_station_id << std::endl;
    }*/
    file.close();
    return states;
}


BodyState CSVReader::read_line_add(std::string& line) {
    BodyState data;
    std::stringstream ss(line);
    std::string field;
    std::vector<std::string> fields;

    while (std::getline(ss, field, ',')) {
        fields.push_back(field);
    }

    if (fields.size() == 8) {
        data.time = std::stod(fields[0]) / day;
        data.position.x = std::stod(fields[2]);
        data.position.y = std::stod(fields[3]);
        data.position.z = std::stod(fields[4]);
        data.velocity.x = std::stod(fields[5]);
        data.velocity.y = std::stod(fields[6]);
        data.velocity.z = std::stod(fields[7]);
    }

    return data;
}