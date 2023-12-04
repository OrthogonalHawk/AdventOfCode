#include <algorithm>
#include <deque>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include "utils.h"

struct GameCard
{
    int game_id;
    std::vector<int> winning_vals;
    std::vector<int> owned_vals;
    int num_winning_numbers;
};

GameCard parseSingleInputStr(const std::string& input)
{
    GameCard ret;

    // Find the position of the ':' character
    size_t colon_pos = input.find(':');

    if (colon_pos != std::string::npos) {
        // Extract the substring before the ':' to get the card number
        std::string card_info = input.substr(input.find("Card") + 5, colon_pos - (input.find("Card") + 5));
        ret.game_id = std::stoi(card_info);

        // Find the position of the '|' character
        size_t separator_pos = input.find('|');

        if (separator_pos != std::string::npos) {
            // Extract the substring after the ':'
            std::string winning_part = input.substr(colon_pos + 1, separator_pos - (colon_pos + 1));
            std::string owned_part = input.substr(separator_pos + 1);

            // Use string stream to parse the integers from the substrings
            std::istringstream winning_stream(winning_part);
            std::istringstream owned_stream(owned_part);

            // Parse and store integers in the winning_vals vector
            int num;
            while (winning_stream >> num) {
                ret.winning_vals.push_back(num);
            }

            // Parse and store integers in the owned_vals vector
            while (owned_stream >> num) {
                ret.owned_vals.push_back(num);
            }
        }
    }

    ret.num_winning_numbers = 0;
    for (const auto& winning_num : ret.winning_vals)
    {
        if (std::find(ret.owned_vals.begin(), ret.owned_vals.end(), winning_num) != ret.owned_vals.end())
        {
            ret.num_winning_numbers++;
        }
    }

    return ret;
}

int compute_part01_score_for_game(const std::string& game_line)
{
    auto game_card = parseSingleInputStr(game_line);

    int score = 0;
    int num_winning_numbers = game_card.num_winning_numbers;
    if (num_winning_numbers > 0)
    {
        score = 1;
        num_winning_numbers--;
    }

    while (num_winning_numbers > 0)
    {
        score *= 2;
        num_winning_numbers--;
    }

    return score;
}

std::map<int, GameCard> create_game_card_map(const std::vector<std::string>& input_strs)
{
    std::map<int, GameCard> ret;

    for (const auto& line : input_strs)
    {
        auto game_card = parseSingleInputStr(line);
        ret[game_card.game_id] = game_card;
    }

    return ret;
}

int main(int argc, char *argv[])
{
    CommandLineOptions options = parse_command_line(argc, argv);
    if (options.helpRequested)
    {
        print_command_line_usage(argv);
        return 1;
    }

    std::string example01_data_file_path = options.dataDir + "/day04/example01.txt";
    std::string puzzle_data_file_path = options.dataDir + "/day04/puzzle.txt";
    std::vector<std::string> parsed_example01_strs = parse_input_file_into_lines(example01_data_file_path);
    std::vector<std::string> puzzle_input_strs = parse_input_file_into_lines(puzzle_data_file_path);

    std::cout << "########## Day04 Example 01 ##########" << std::endl;

    auto example_01_game_score_sum = 0;
    for (const auto& game : parsed_example01_strs)
    {
        example_01_game_score_sum += compute_part01_score_for_game(game);
    }

    std::cout << "Game Sum: " << example_01_game_score_sum << std::endl;

    std::cout << "########## Day04 Part 01 #############" << std::endl;

    auto part_01_game_score_sum = 0;
    for (const auto& game : puzzle_input_strs)
    {
        part_01_game_score_sum += compute_part01_score_for_game(game);
    }

    std::cout << "Game Sum: " << part_01_game_score_sum << std::endl;

    std::cout << "########## Day04 Part 02 #############" << std::endl;

    auto game_card_map = create_game_card_map(puzzle_input_strs);

    int part_02_num_game_cards = 0;
    std::deque<GameCard> cards_to_process;

    // get the initial set of cards to process; one for every input
    for (const auto& card : game_card_map)
    {
        cards_to_process.push_back(card.second);
    }

    while (cards_to_process.size() > 0)
    {
        part_02_num_game_cards++;

        auto next_card = cards_to_process.front();
        if (next_card.num_winning_numbers > 0)
        {
            for (int ii = 1; ii <= next_card.num_winning_numbers; ++ii)
            {
                cards_to_process.push_back(game_card_map[next_card.game_id + ii]);
            }
        }

        cards_to_process.pop_front();
    }

    std::cout << "Num Cards: " << part_02_num_game_cards << std::endl;



    return 0;
}
