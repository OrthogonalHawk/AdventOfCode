#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "utils.h"

CommandLineOptions parse_command_line(int argc, char *argv[])
{
    CommandLineOptions options;

    for (int ii = 1; ii < argc; ++ii)
    {
        std::string arg = argv[ii];

        if (arg == "--data-dir") {
            // Ensure there is a subsequent argument available
            if (ii + 1 < argc) {
                options.dataDir = argv[ii + 1];
                ++ii;  // Skip the next argument since it has been consumed
            } else {
                std::cerr << "Error: --data-dir option requires a path argument.\n";
                options.helpRequested = true;
            }
        } else if (arg == "--help") {
            options.helpRequested = true;
        } else {
            std::cerr << "Error: Unsupported option '" << arg << "'.\n";
            options.helpRequested = true;
        }
    }

    return options;
}

void print_command_line_usage(char *argv[]) {
    std::cout << "Usage: " << argv[0] << " [options]\n"
              << "Supported options:\n"
              << "  --data-dir <path>   Specify the path to the 'data' directory.\n"
              << "  --help              Show this help message.\n";
}

std::vector<std::string> parse_input_file_into_lines(const std::string& filePath)
{
    std::vector<std::string> result;
    std::ifstream inputFile(filePath);
    std::string line;

    if (inputFile.is_open()) {
        while (std::getline(inputFile, line)) {
            if (line.empty()) {
                result.push_back("");
            } else {
                result.push_back(line);
            }
        }
        inputFile.close();
    } else {
        std::cerr << "Unable to open file: " << filePath << std::endl;
    }

    return result;
}
