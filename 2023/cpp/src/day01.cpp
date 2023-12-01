#include <cstring>
#include <iostream>
#include <sstream>

#include "utils.h"

int sum_calibration_values_by_numeric_digits(std::vector<std::string>& vals)
{
    int accum_sum = 0;
    for (const auto& line : vals)
    {
        std::stringstream ss;
        for (int ii = 0; ii < line.size(); ++ii)
        {
            if (std::isdigit(line[ii]))
            {
                ss << line[ii];
                break;
            }
        }
        for (int ii = line.size() - 1; ii >= 0; --ii)
        {
            if (std::isdigit(line[ii]))
            {
                ss << line[ii];
                break;
            }
        }

        int calibration_val = 0;
        ss >> calibration_val;

        accum_sum += calibration_val;
    }

    return accum_sum;
}

struct SearchVal
{
    int numeric_val;
    std::vector<std::string> search_strs;
};

int sum_calibration_values_by_numeric_digits_or_number_strs(std::vector<std::string>& vals)
{
    std::vector<SearchVal> SEARCH_VALS = {
        {1, {"1", "one"}},
        {2, {"2", "two"}},
        {3, {"3", "three"}},
        {4, {"4", "four"}},
        {5, {"5", "five"}},
        {6, {"6", "six"}},
        {7, {"7", "seven"}},
        {8, {"8", "eight"}},
        {9, {"9", "nine"}},
    };

    int accum_sum = 0;
    int num_processed = 0;
    for (const auto& line : vals)
    {
        std::string::size_type early_start_pos = 0xFFFFFFFF;
        int early_start_pos_val = 0;

        std::string::size_type late_start_pos = 0;
        int late_start_pos_val = 0;

        for (const auto& num : SEARCH_VALS)
        {
            for (const auto& search_str : num.search_strs)
            {
                auto first_start_pos = line.find(search_str);
                std::string::size_type tmp_last_start_pos = 0;
                std::string::size_type last_start_pos = std::string::npos;
                while( std::string::npos != 
                       ( tmp_last_start_pos = line.find( search_str, tmp_last_start_pos ) ) )
                {
                    last_start_pos = tmp_last_start_pos;
                    tmp_last_start_pos++;
                }

                if (first_start_pos != std::string::npos)
                {
                    if (first_start_pos < early_start_pos)
                    {
                        early_start_pos = first_start_pos;
                        early_start_pos_val = num.numeric_val;
                    }
                }

                if (last_start_pos != std::string::npos)
                {
                    if (last_start_pos > late_start_pos ||
                        late_start_pos == 0)
                    {
                        late_start_pos = last_start_pos;
                        late_start_pos_val = num.numeric_val;
                    }
                }
            }
        }

        std::stringstream ss;
        ss << early_start_pos_val << late_start_pos_val;
        
        int calibration_val = 0;
        ss >> calibration_val;

        accum_sum += calibration_val;
    }

    return accum_sum;
}

int main(int argc, char *argv[])
{
    CommandLineOptions options = parse_command_line(argc, argv);
    if (options.helpRequested)
    {
        print_command_line_usage(argv);
        return 1;
    }

    std::string example01_data_file_path = options.dataDir + "/day01/example01.txt";
    std::string example02_data_file_path = options.dataDir + "/day01/example02.txt";
    std::string example03_data_file_path = options.dataDir + "/day01/example03.txt";
    std::string puzzle_data_file_path = options.dataDir + "/day01/puzzle.txt";
    std::vector<std::string> parsed_example01_strs = parse_input_file_into_lines(example01_data_file_path);
    std::vector<std::string> parsed_example02_strs = parse_input_file_into_lines(example02_data_file_path);
    std::vector<std::string> parsed_example03_strs = parse_input_file_into_lines(example03_data_file_path);
    std::vector<std::string> puzzle_input_strs = parse_input_file_into_lines(puzzle_data_file_path);

    std::cout << "########## Day01 Example 01 ##########" << std::endl;

    auto example_accum_sum = sum_calibration_values_by_numeric_digits(parsed_example01_strs);
    std::cout << "Calibration Value Sum: " << example_accum_sum << std::endl;

    std::cout << "########## Day01 Part 1 ###########" << std::endl;

    auto part01_accum_sum = sum_calibration_values_by_numeric_digits(puzzle_input_strs);
    std::cout << "Calibration Value Sum: " << part01_accum_sum << std::endl;

    std::cout << "########## Day01 Example 02 ##########" << std::endl;
    auto part02_example_accum_sum = sum_calibration_values_by_numeric_digits_or_number_strs(parsed_example02_strs);
    std::cout << "Calibration Value Sum: " << part02_example_accum_sum << std::endl;

    std::cout << "########## Day01 Example 03 ##########" << std::endl;
    auto part03_example_accum_sum = sum_calibration_values_by_numeric_digits_or_number_strs(parsed_example03_strs);
    std::cout << "Calibration Value Sum: " << part03_example_accum_sum << std::endl;

    std::cout << "########## Day01 Part 2 ###########" << std::endl;
    auto part02_accum_sum = sum_calibration_values_by_numeric_digits_or_number_strs(puzzle_input_strs);
    std::cout << "Calibration Value Sum: " << part02_accum_sum << std::endl;

    return 0;
}