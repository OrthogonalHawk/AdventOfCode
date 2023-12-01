#include <iostream>

#include "conversion.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    CommandLineOptions options = parse_command_line(argc, argv);
    if (options.helpRequested)
    {
        print_command_line_usage(argv);
        return 1;
    }

    std::string data_file_path = options.dataDir + "/day00/example.txt";
    std::vector<std::string> parsed_strs = parse_input_file_into_lines(data_file_path);
    auto parsed_ints = convert_string_vector_to_numbers<uint32_t>(parsed_strs, 0xFFFFFFFF);

    for (const auto& line : parsed_strs)
    {
        std::cout << line << std::endl;
    }

    std::cout << "####################" << std::endl;
    
    for (const auto& num : parsed_ints)
    {
        std::cout << num << std::endl;
    }

    return 0;
}