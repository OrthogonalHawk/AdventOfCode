#include <iostream>
#include <fstream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <map>

#include "utils.h"

struct Waypoint {
    std::string left;
    std::string right;
};

std::string parseInstructions(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file " << filename << std::endl;
        return "";
    }

    std::string line;
    if (std::getline(file, line)) {
        return line;
    } else {
        std::cerr << "File is empty: " << filename << std::endl;
        return "";
    }
}

std::map<std::string, Waypoint> parseWaypoints(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file " << filename << std::endl;
        return {};
    }

    std::map<std::string, Waypoint> waypoints;
    std::string line;

    // Skip the first line containing instructions
    if (std::getline(file, line))
    {
        while (std::getline(file, line))
        {
            if (line.size() <= 1)
            {
                continue;
            }
            std::istringstream iss(line);
            std::string key, equalSign, left, comma, right;
            
            if (!(iss >> key >> std::ws >> equalSign >> std::ws >> left >> std::ws >> right)) {
                std::cerr << "Error parsing line: " << line << std::endl;
                continue;
            }

            // Remove the trailing comma and parentheses from left and right strings
            left.erase(left.find('('), 1);
            left.erase(left.find(','), 1);
            right.pop_back();

            waypoints[key] = {left, right};
        }
    } else {
        std::cerr << "File is empty: " << filename << std::endl;
    }

    return waypoints;
}

int find_part01_path_to_finish(const std::string& instructions, std::map<std::string, Waypoint>& map, std::string start_pos)
{
    std::string cur_pos = start_pos;
    int num_moves = 0;
    int next_instruction_idx = 0;

    while (cur_pos != "ZZZ")
    {
        char next_instruction = instructions[next_instruction_idx++];
        if (next_instruction_idx >= instructions.size())
        {
            next_instruction_idx = 0;
        }

        if (next_instruction == 'L')
        {
            cur_pos = map[cur_pos].left;
        }
        else
        {
            cur_pos = map[cur_pos].right;
        }

        num_moves++;
    }

    return num_moves;
}

std::vector<std::string> find_ghost_start_nodes(const std::map<std::string, Waypoint>& map)
{
    std::vector<std::string> ret;

    for (const auto& iter : map)
    {
        if (iter.first[2] == 'A')
        {
            ret.push_back(iter.first);
        }
    }

    return ret;
}

void threaded_find_path_to_ghost_finish(const std::string& instructions, const std::map<std::string, Waypoint>& waypoint_map, std::string start_pos, std::vector<int64_t>& resultQueue, std::mutex& resultMutex)
{
    std::string cur_pos = start_pos;
    int64_t num_moves = 0;
    int next_instruction_idx = 0;

    bool found_destination = false;
    while (!found_destination)
    {
        char next_instruction = instructions[next_instruction_idx++];
        if (next_instruction_idx >= instructions.size())
        {
            next_instruction_idx = 0;
        }

        if (next_instruction == 'L')
        {
            cur_pos = waypoint_map.at(cur_pos).left;
        }
        else
        {
            cur_pos = waypoint_map.at(cur_pos).right;
        }

        num_moves++;

        if (cur_pos[2] == 'Z')
        {
            found_destination = true;
        }
    }

    std::cout << "Finished after " << num_moves << " moves" << std::endl;

    // Push result to the resultQueue (thread-safe)
    std::lock_guard<std::mutex> lock(resultMutex);
    resultQueue.push_back(num_moves);
}

int find_part02_path_to_finish(const std::string& instructions, const std::map<std::string, Waypoint>& waypoint_map)
{
    std::vector<std::string> cur_pos = find_ghost_start_nodes(waypoint_map);

    std::vector<std::thread> threads;
    std::vector<int64_t> resultQueue;
    std::mutex resultMutex;

    std::cout << "Starting from " << cur_pos.size() << " nodes" << std::endl;

    for (int ii = 0; ii < cur_pos.size(); ++ii)
    {
        std::string start_pos = cur_pos[ii];
        threads.emplace_back([instructions, waypoint_map, start_pos, &resultQueue, &resultMutex]()
        {
            threaded_find_path_to_ghost_finish(instructions, waypoint_map, start_pos, resultQueue, resultMutex);
        });
    }

    // Wait for all threads to finish their work
    for (auto& thread : threads) {
        thread.join();
    }
    threads.clear();

    return 0;
}

int main(int argc, char *argv[])
{
    CommandLineOptions options = parse_command_line(argc, argv);
    if (options.helpRequested)
    {
        print_command_line_usage(argv);
        return 1;
    }

    std::string example01_data_file_path = options.dataDir + "/day08/example01.txt";
    std::string example02_data_file_path = options.dataDir + "/day08/example02.txt";
    std::string example03_data_file_path = options.dataDir + "/day08/example03.txt";
    std::string puzzle_data_file_path = options.dataDir + "/day08/puzzle.txt";

    std::cout << "########## Day08 Example 01 ##########" << std::endl;

    std::string example01_instructions = parseInstructions(example01_data_file_path);
    std::map<std::string, Waypoint> example01_waypoints = parseWaypoints(example01_data_file_path);

    std::cout << "Takes: " << find_part01_path_to_finish(example01_instructions, example01_waypoints, "AAA") << " steps to reach the finish" << std::endl;

    std::cout << "########## Day08 Example 02 ##########" << std::endl;

    std::string example02_instructions = parseInstructions(example02_data_file_path);
    std::map<std::string, Waypoint> example02_waypoints = parseWaypoints(example02_data_file_path);

    std::cout << "Takes: " << find_part01_path_to_finish(example02_instructions, example02_waypoints, "AAA") << " steps to reach the finish" << std::endl;

    std::cout << "########## Day08 Example 03 ##########" << std::endl;

    std::string example03_instructions = parseInstructions(example03_data_file_path);
    std::map<std::string, Waypoint> example03_waypoints = parseWaypoints(example03_data_file_path);

    std::cout << "Takes: " << find_part02_path_to_finish(example03_instructions, example03_waypoints) << " steps to reach the finish" << std::endl;

    std::cout << "########## Day08 Part 01 #############" << std::endl;

    std::string part01_instructions = parseInstructions(puzzle_data_file_path);
    std::map<std::string, Waypoint> part01_waypoints = parseWaypoints(puzzle_data_file_path);

    std::cout << "Takes: " << find_part01_path_to_finish(part01_instructions, part01_waypoints, "AAA") << " steps to reach the finish" << std::endl;

    std::cout << "########## Day08 Part 02 #############" << std::endl;

    std::string part02_instructions = parseInstructions(puzzle_data_file_path);
    std::map<std::string, Waypoint> part02_waypoints = parseWaypoints(puzzle_data_file_path);

    std::cout << "Takes: " << find_part02_path_to_finish(part02_instructions, part02_waypoints) << " steps to reach the finish" << std::endl;

    // >>> import math
    // >>> print(math.lcm(<values>)

    return 0;
}
