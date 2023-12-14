#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "utils.h"

struct Race {
    int64_t race_time;
    int64_t record_distance;

    int64_t find_shortest_button_hold_for_victory(void) const
    {
        for (int64_t hold_time = 1; hold_time < race_time; hold_time++)
        {
            int64_t remaining_race_time = race_time - hold_time;
            int64_t distance = remaining_race_time * hold_time;

            if (distance > record_distance)
            {
                return hold_time;
            }
        }

        return -1;
    }

    int64_t find_longest_button_hold_for_victory(void) const
    {
        for (int64_t hold_time = (race_time-1); hold_time > 0; hold_time--)
        {
            int64_t remaining_race_time = race_time - hold_time;
            int64_t distance = remaining_race_time * hold_time;

            if (distance > record_distance)
            {
                return hold_time;
            }
        }

        return -1;
    }
};

std::vector<Race> parseInputFile(const std::string& filename) {
    std::vector<Race> races;
    std::vector<int64_t> times;
    std::vector<int64_t> distances;

    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return races;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string key;
        iss >> key;

        if (key == "Time:" || key == "Distance:") {
            int64_t value;
            while (iss >> value) {
                if (key == "Time:") {
                    times.push_back(value);
                } else {
                    distances.push_back(value);
                }
            }
        }
    }

    inputFile.close();

    // Combine record times and distances into Race structures
    size_t minSize = std::min(times.size(), distances.size());
    for (size_t i = 0; i < minSize; ++i) {
        Race race;
        race.race_time = times[i];
        race.record_distance = distances[i];
        races.push_back(race);
    }

    return races;
}

std::vector<Race> parseSingleRaceInputFile(const std::string& filename) {
    std::vector<Race> races;
    std::vector<int64_t> times;
    std::vector<int64_t> distances;

    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return races;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string key;
        iss >> key;

        if (key == "Time:" || key == "Distance:") {
            int64_t value = 0;
            char c;
            while (iss >> c) {
                if (std::isdigit(c)) {
                    value = value * 10 + (c - '0');
                }
            }

            if (key == "Time:") {
                times.push_back(value);
            } else {
                distances.push_back(value);
            }
        }
    }

    inputFile.close();

    // Combine record times and distances into Race structures
    size_t minSize = std::min(times.size(), distances.size());
    for (size_t i = 0; i < minSize; ++i) {
        Race race;
        race.race_time = times[i];
        race.record_distance = distances[i];
        races.push_back(race);
    }

    return races;
}

int main(int argc, char *argv[])
{
    CommandLineOptions options = parse_command_line(argc, argv);
    if (options.helpRequested)
    {
        print_command_line_usage(argv);
        return 1;
    }

    std::string example01_data_file_path = options.dataDir + "/day06/example01.txt";
    std::string puzzle_data_file_path = options.dataDir + "/day06/puzzle.txt";

    std::cout << "########## Day06 Example 01 ##########" << std::endl;

    auto example01_races = parseInputFile(example01_data_file_path);
    int64_t example01_accum = 1;

    for (const auto& race : example01_races)
    {
        int64_t num_ways_to_win = (race.find_longest_button_hold_for_victory() - race.find_shortest_button_hold_for_victory() + 1);
        std::cout << "Can win " << num_ways_to_win << " different ways" << std::endl;
        example01_accum *= num_ways_to_win;
    }
    std::cout << "Accum Race Options: " << example01_accum << std::endl;

    std::cout << "########## Day06 Part 01 #############" << std::endl;

    auto part01_races = parseInputFile(puzzle_data_file_path);
    int64_t part01_accum = 1; 

    for (const auto& race : part01_races)
    {
        int num_ways_to_win = (race.find_longest_button_hold_for_victory() - race.find_shortest_button_hold_for_victory() + 1);
        std::cout << "Can win " << num_ways_to_win << " different ways" << std::endl;
        part01_accum *= num_ways_to_win;
    }
    std::cout << "Accum Race Options: " << part01_accum << std::endl;

    std::cout << "########## Day06 Part 02 #############" << std::endl;

    auto part02_races = parseSingleRaceInputFile(puzzle_data_file_path);
    int64_t part02_accum = 1;

    for (const auto& race : part02_races)
    {
        int64_t num_ways_to_win = (race.find_longest_button_hold_for_victory() - race.find_shortest_button_hold_for_victory() + 1);
        std::cout << "Can win " << num_ways_to_win << " different ways" << std::endl;
        part02_accum *= num_ways_to_win;
    }
    std::cout << "Accum Race Options: " << part02_accum << std::endl;

    return 0;
}
