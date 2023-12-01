#ifndef ADVENT_OF_CODE_UTILS_H
#define ADVENT_OF_CODE_UTILS_H

#include <iostream>
#include <string>
#include <vector>

struct CommandLineOptions {
    std::string dataDir;
    bool helpRequested = false;
};

std::vector<std::string> parse_input_file_into_lines(const std::string& filePath);
CommandLineOptions parse_command_line(int argc, char *argv[]);
void print_command_line_usage(char *argv[]);

#endif /* ADVENT_OF_CODE_UTILS_H */