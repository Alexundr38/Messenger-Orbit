//
// Created by sun on 08.12.2025.
//

#include "CSV_reader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>  // для std::get_time

struct CSVRow {
    long long time_tag_seconds;
    std::string record_time;
    double full_observable;
    int receiving_station_id;
    double full_ref_freq;
};

// Функция для разбора строки CSV
CSVRow parseCSVRow(const std::string& line) {
    CSVRow row;
    std::stringstream ss(line);
    std::string token;
    std::vector<std::string> tokens;

    // Разбиваем строку по запятым
    while (std::getline(ss, token, ',')) {
        tokens.push_back(token);
    }

    // Заполняем структуру (индексы соответствуют заголовку)
    if (tokens.size() >= 13) {  // Проверяем, что достаточно столбцов
        try {
            // time_tag_seconds
            row.time_tag_seconds = std::stoll(tokens[0]);

            // record_time (оставляем как строку)
            row.record_time = tokens[1];

            // full_observable
            row.full_observable = std::stod(tokens[2]);

            // receiving_station_id
            row.receiving_station_id = std::stoi(tokens[5]);

            // full_ref_freq
            row.full_ref_freq = std::stod(tokens[8]);
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка парсинга строки: " << e.what() << std::endl;
            std::cerr << "Проблемная строка: " << line << std::endl;
            throw;
        }
    }

    return row;
}

// Функция для чтения всего CSV файла
std::vector<CSVRow> readCSVFile(const std::string& filename) {
    std::vector<CSVRow> data;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл: " + filename);
    }

    std::string line;
    bool is_first_line = true;

    while (std::getline(file, line)) {
        // Пропускаем первую строку (заголовок)
        if (is_first_line) {
            is_first_line = false;
            continue;
        }

        // Пропускаем пустые строки
        if (line.empty()) {
            continue;
        }

        try {
            CSVRow row = parseCSVRow(line);
            data.push_back(row);
        }
        catch (const std::exception& e) {
            std::cerr << "Пропускаю строку из-за ошибки: " << line << std::endl;
        }
    }

    file.close();
    return data;
}