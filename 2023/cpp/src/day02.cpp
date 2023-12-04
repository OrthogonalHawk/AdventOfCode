#include <algorithm>
#include <cstring>
#include <iostream>
#include <numeric>
#include <sstream>

#include "utils.h"

struct ColorReveals {
    int num_red = 0;
    int num_blue = 0;
    int num_green = 0;
};

struct GameParams {
    int id;
    std::vector<ColorReveals> colorReveals;
};

std::vector<GameParams> parse_game_data(const std::vector<std::string>& input)
{
    std::vector<GameParams> game_params;

    for (const auto& line : input)
    {
        GameParams params;
        std::istringstream iss(line);

        // Extracting the "Game <id>:" portion
        std::string game_id_section;
        std::getline(iss, game_id_section, ':');

        // Extracting the ID number
        game_id_section.erase(0, 5); // Removing "Game " from the string
        params.id = std::stoi(game_id_section);

        // Extracting semi-colon separated sections
        std::string section;
        while (std::getline(iss, section, ';'))
        {
            ColorReveals color;
            std::istringstream sectionStream(section);

            // Extracting the count and color
            int count;
            std::string colorStr;
            while (sectionStream >> count >> std::ws >> colorStr)
            {
                // remove any commas if they exist
                colorStr.erase(std::remove(colorStr.begin(), colorStr.end(), ','), colorStr.end());

                // Populating the ColorReveals structure based on color
                if (colorStr == "red")
                {
                    color.num_red = count;
                }
                else if (colorStr == "blue")
                {
                    color.num_blue = count;
                }
                else if (colorStr == "green")
                {
                    color.num_green = count;
                }
            }
            // std::cout << "Game " << params.id << " Reveal " << params.colorReveals.size() << " red " << color.num_red << " blue " << color.num_blue << " green " << color.num_green << std::endl;
            params.colorReveals.push_back(color);
        }

        game_params.push_back(params);
    }

    return game_params;
}

std::vector<int> eval_possible_games_given_limits(std::vector<GameParams>& game_params, int max_red, int max_blue, int max_green)
{
    std::vector<int> possible_game_ids;

    for (const auto& game : game_params)
    {
        bool exceeded_limits = false;
        for (const auto& reveal : game.colorReveals)
        {
            if (reveal.num_red > max_red ||
                reveal.num_blue > max_blue ||
                reveal.num_green > max_green)
            {
                exceeded_limits = true;
                break;
            }
        }

        if (!exceeded_limits)
        {
            possible_game_ids.push_back(game.id);
        }
    }

    return possible_game_ids;
}

std::vector<ColorReveals> find_min_colors_for_possible_game(std::vector<GameParams>& game_params)
{
    std::vector<ColorReveals> min_colors;

    for (const auto& game : game_params)
    {
        ColorReveals next_min_colors;

        for (const auto& reveal : game.colorReveals)
        {
            if (reveal.num_red > next_min_colors.num_red)
            {
                next_min_colors.num_red = reveal.num_red;
            }
            if (reveal.num_blue > next_min_colors.num_blue)
            {
                next_min_colors.num_blue = reveal.num_blue;
            }
            if (reveal.num_green > next_min_colors.num_green)
            {
                next_min_colors.num_green = reveal.num_green;
            }
        }

        min_colors.push_back(next_min_colors);
    }

    return min_colors;
}

int calculate_min_color_power_sum(const std::vector<ColorReveals>& colors)
{
    int power_sum = 0;

    for (const auto& color : colors)
    {
        power_sum += color.num_red * color.num_blue * color.num_green;
    }

    return power_sum;
}

int main(int argc, char *argv[])
{
    CommandLineOptions options = parse_command_line(argc, argv);
    if (options.helpRequested)
    {
        print_command_line_usage(argv);
        return 1;
    }

    std::string example01_data_file_path = options.dataDir + "/day02/example01.txt";
    std::string puzzle_data_file_path = options.dataDir + "/day02/puzzle.txt";
    std::vector<std::string> parsed_example01_strs = parse_input_file_into_lines(example01_data_file_path);
    std::vector<std::string> puzzle_input_strs = parse_input_file_into_lines(puzzle_data_file_path);

    std::cout << "########## Day02 Example 01 ##########" << std::endl;

    auto example_01_game_params = parse_game_data(parsed_example01_strs);
    auto example_01_possible_game_ids_given_limits = eval_possible_games_given_limits(example_01_game_params, 12, 14, 13);
    auto example_01_game_id_sum = std::accumulate(example_01_possible_game_ids_given_limits.begin(), example_01_possible_game_ids_given_limits.end(), 0);
    std::cout << "Possible Game Id Sum: " << example_01_game_id_sum << std::endl;

    auto example_01_min_colors_for_possible_game = find_min_colors_for_possible_game(example_01_game_params);
    auto example_01_color_power_sum = calculate_min_color_power_sum(example_01_min_colors_for_possible_game);
    std::cout << "Color Power Sum: " << example_01_color_power_sum << std::endl;

    std::cout << "########## Day02 Part 1 ###########" << std::endl;

    auto part_01_game_params = parse_game_data(puzzle_input_strs);
    auto part_01_possible_game_ids = eval_possible_games_given_limits(part_01_game_params, 12, 14, 13);
    auto part_01_game_id_sum = std::accumulate(part_01_possible_game_ids.begin(), part_01_possible_game_ids.end(), 0);
    std::cout << "Possible Game Id Sum: " << part_01_game_id_sum << std::endl;

    std::cout << "########## Day02 Part 2 ###########" << std::endl;
    auto part_02_min_colors_for_possible_game = find_min_colors_for_possible_game(part_01_game_params);
    auto part_02_color_power_sum = calculate_min_color_power_sum(part_02_min_colors_for_possible_game);
    std::cout << "Color Power Sum: " << part_02_color_power_sum << std::endl;

    return 0;
}