#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "utils.h"

// Function to parse a line into a vector of integers
std::vector<int> parseLine(const std::string& line) {
    std::vector<int> result;
    std::istringstream iss(line);
    int num;
    while (iss >> num) {
        result.push_back(num);
    }
    return result;
}

// Function to read the input file and parse each line into a vector of integers
std::vector<std::vector<int>> parseFile(const std::string& filename) {
    std::vector<std::vector<int>> lines;
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::vector<int> nums = parseLine(line);
            lines.push_back(nums);
        }
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
    return lines;
}

void printIntVec(const std::vector<int>& vec)
{
    for (int num : vec) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

bool allZeros(const std::vector<int>& vec) {
    return std::all_of(vec.begin(), vec.end(), [](int val) { return val == 0; });
}

int part01_calculate_next_val(const std::vector<int>& history)
{
    std::vector<std::vector<int>> histories = {history};
    while (!allZeros(*histories.rbegin()))
    {
        const auto prev_history = *histories.rbegin();
        std::vector<int> next_history;
        for (auto ii = 1; ii < prev_history.size(); ++ii)
        {
            next_history.push_back(prev_history[ii] - prev_history[ii - 1]);
        }
        histories.push_back(next_history);
    }

    for (int ii = (histories.size() - 2); ii >= 0; --ii)
    {
        histories[ii].push_back(*histories[ii + 1].rbegin() + *histories[ii].rbegin());
    }

    return *(*histories.begin()).rbegin();
}

int part02_calculate_prev_val(const std::vector<int>& history)
{
    std::vector<std::vector<int>> histories = {history};

    while (!allZeros(*histories.rbegin()))
    {
        const auto prev_history = *histories.rbegin();
        std::vector<int> next_history;
        for (auto ii = 1; ii < prev_history.size(); ++ii)
        {
            next_history.push_back(prev_history[ii] - prev_history[ii - 1]);
        }
        histories.push_back(next_history);
    }

    for (int ii = (histories.size() - 2); ii >= 0; --ii)
    {
        histories[ii].insert(histories[ii].begin(), *histories[ii].begin() - *histories[ii + 1].begin());
    }

    return *(*histories.begin()).begin();
}

int main(int argc, char *argv[])
{
    CommandLineOptions options = parse_command_line(argc, argv);
    if (options.helpRequested)
    {
        print_command_line_usage(argv);
        return 1;
    }

    std::string example01_data_file_path = options.dataDir + "/day09/example01.txt";
    std::string puzzle_data_file_path = options.dataDir + "/day09/puzzle.txt";
    std::vector<std::vector<int>> example_01_vals = parseFile(example01_data_file_path);
    std::vector<std::vector<int>> puzzle_vals = parseFile(puzzle_data_file_path);

    std::cout << "########## Day09 Example 01 ##########" << std::endl;

    int example01_next_val_sum = 0;
    for (auto history : example_01_vals)
    {
        example01_next_val_sum += part01_calculate_next_val(history);
    }

    std::cout << "Next Value Sum: " << example01_next_val_sum << std::endl;

    int example01_prev_val_sum = 0;
    for (auto history : example_01_vals)
    {
        example01_prev_val_sum += part02_calculate_prev_val(history);
    }

    std::cout << "Prev Value Sum: " << example01_prev_val_sum << std::endl;

    std::cout << "########## Day09 Part 01 #############" << std::endl;

    int part01_next_val_sum = 0;
    for (auto history : puzzle_vals)
    {
        part01_next_val_sum += part01_calculate_next_val(history);
    }

    std::cout << "Next Value Sum: " << part01_next_val_sum << std::endl;

    std::cout << "########## Day09 Part 02 #############" << std::endl;

    int part02_prev_val_sum = 0;
    for (auto history : puzzle_vals)
    {
        part02_prev_val_sum += part02_calculate_prev_val(history);
    }

    std::cout << "Prev Value Sum: " << part02_prev_val_sum << std::endl;

    return 0;
}
