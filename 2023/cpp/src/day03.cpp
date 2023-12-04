#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <set>
#include <sstream>
#include <string>

#include "utils.h"

enum ParserState { SEARCH_FOR_NEXT_ENTRY, PARSING_NUMBER };

struct Location
{
    int x;
    int y;

    // Custom comparison operator for strict weak ordering
    bool operator<(const Location& rhs) const
    {
        if (x == rhs.x)
        {
            return y < rhs.y;
        }
        return x < rhs.x;
    }

    // Custom equality operator
    inline bool operator==(const Location& rhs)
    {
        return x == rhs.x && y == rhs.y;
    }

    std::string to_str(void)
    {
        std::stringstream ss;
        ss << "{" << x << "," << y << "}";
        return ss.str();
    }
};

struct ParsedVal {
    std::string str;
    Location loc;
    bool is_symbol;
    int numeric_val = 0;

    void print_str(void) const
    {
        std::cout << "{ \"" << str << "\", (" << loc.x
                  << "," << loc.y << ") }\n";
    }

    std::set<Location> get_adjacent_locations(int max_x, int max_y) const
    {
        std::set<Location> ret;

        for (int x_pos = loc.x; x_pos < (loc.x + str.size()); ++x_pos)
        {
            if (x_pos > 0)
            {
                // left
                ret.insert({x_pos - 1, loc.y});
            }

            if (x_pos > 0 && loc.y < max_y)
            {
                // upper left
                ret.insert({x_pos - 1, loc.y + 1});
            }

            if (loc.y < max_y)
            {
                // up
                ret.insert({x_pos, loc.y + 1});
            }

            if (x_pos < max_x && loc.y < max_y)
            {
                // up right
                ret.insert({x_pos + 1, loc.y + 1});
            }

            if (x_pos < max_x)
            {
                // right
                ret.insert({x_pos + 1, loc.y});
            }

            if (x_pos < max_x && loc.y > 0)
            {
                // right down
                ret.insert({x_pos + 1, loc.y - 1});
            }

            if (loc.y > 0)
            {
                // down
                ret.insert({x_pos, loc.y - 1});
            }

            if (loc.y > 0 && x_pos > 0)
            {
                // down left
                ret.insert({x_pos - 1, loc.y - 1});
            }
        }

        return ret;
    }
};

void parseInput(const std::vector<std::string>& input, std::vector<ParsedVal>& vals, std::vector<ParsedVal>& symbols)
{
    int line_idx = 0;
    for (auto line_iter = input.rbegin(); line_iter != input.rend(); ++line_iter)
    {
        std::string line = *line_iter;

        int startPos = 0;
        int currentPos = 0;
        bool isSymbol = false;

        auto parser_state = ParserState::SEARCH_FOR_NEXT_ENTRY;
        std::stringstream next_str;
        while (currentPos < line.length())
        {
            switch (parser_state)
            {
            case ParserState::SEARCH_FOR_NEXT_ENTRY:
                if (line[currentPos] != '.')
                {
                    if (std::isdigit(line[currentPos]))
                    {
                        // starting to parse a number
                        startPos = currentPos;
                        isSymbol = false;
                        
                        next_str.str(std::string());
                        next_str << line[currentPos];

                        parser_state = ParserState::PARSING_NUMBER;
                    }
                    else
                    {
                        // must be a symbol
                        next_str.str(std::string());
                        next_str << line[currentPos];
                        symbols.push_back({next_str.str(), {currentPos, line_idx}, true, 0});

                        next_str.str(std::string());
                    }
                }
                break;

            case ParserState::PARSING_NUMBER:
                if (line[currentPos] == '.')
                {
                    // previous number is complete
                    vals.push_back({next_str.str(), {startPos, line_idx}, false, std::stoi(next_str.str())});

                    parser_state = ParserState::SEARCH_FOR_NEXT_ENTRY;
                    next_str.str(std::string());
                }
                else if (std::isdigit(line[currentPos]))
                {
                    next_str << line[currentPos];
                }
                else
                {
                    // previous number must be complete and we found a symbol
                    vals.push_back({next_str.str(), {startPos, line_idx}, false, std::stoi(next_str.str())});

                    // handle the symbol
                    next_str.str(std::string());
                    next_str << line[currentPos];
                    symbols.push_back({next_str.str(), {currentPos, line_idx}, true});

                    parser_state = ParserState::SEARCH_FOR_NEXT_ENTRY;
                    next_str.str(std::string());
                }
                break;

            default:
                std::cerr << "Unexpected parser state" << std::endl;
                assert(false);
            }

            currentPos++;
        }

        // check for a number in progress when the loop finishes
        if (ParserState::PARSING_NUMBER == parser_state)
        {
            vals.push_back({next_str.str(), {startPos, line_idx}, false, std::stoi(next_str.str())});
        }

        line_idx++;
    }
}

bool has_adjacent_symbol(const ParsedVal& val, std::vector<ParsedVal>& symbols, int max_x, int max_y)
{
    bool has_adjacent_symbol = false;

    auto possible_symbol_locations = val.get_adjacent_locations(max_x, max_y);
    for (const auto& symbol : symbols)
    {
        if (possible_symbol_locations.find(symbol.loc) != possible_symbol_locations.end())
        {
            has_adjacent_symbol = true;
            break;
        }
    }

    return has_adjacent_symbol;
}

int main(int argc, char *argv[])
{
    CommandLineOptions options = parse_command_line(argc, argv);
    if (options.helpRequested)
    {
        print_command_line_usage(argv);
        return 1;
    }

    std::string example01_data_file_path = options.dataDir + "/day03/example01.txt";
    std::string example02_data_file_path = options.dataDir + "/day03/example02.txt";
    std::string puzzle_data_file_path = options.dataDir + "/day03/puzzle.txt";
    std::vector<std::string> parsed_example01_strs = parse_input_file_into_lines(example01_data_file_path);
    std::vector<std::string> parsed_example02_strs = parse_input_file_into_lines(example02_data_file_path);
    std::vector<std::string> puzzle_input_strs = parse_input_file_into_lines(puzzle_data_file_path);

    std::cout << "########## Day03 Example 01 ##########" << std::endl;

    std::vector<ParsedVal> example_01_vals, example_01_symbols;
    parseInput(parsed_example01_strs, example_01_vals, example_01_symbols);
    int example_01_part_num_sum = 0;
    for (const auto& cand_part : example_01_vals)
    {
        if (has_adjacent_symbol(cand_part, example_01_symbols, parsed_example01_strs[0].size() - 1, parsed_example01_strs.size() - 1))
        {
            example_01_part_num_sum += cand_part.numeric_val;
        }
    }
    std::cout << "Part Number Sum: " << example_01_part_num_sum << std::endl;

    std::cout << "########## Day03 Example 02 ##########" << std::endl;

    std::vector<ParsedVal> example_02_vals, example_02_symbols;
    parseInput(parsed_example02_strs, example_02_vals, example_02_symbols);
    int example_02_part_num_sum = 0;
    for (const auto& cand_part : example_02_vals)
    {
        if (has_adjacent_symbol(cand_part, example_02_symbols, parsed_example02_strs[0].size() - 1, parsed_example02_strs.size() - 1))
        {
            example_02_part_num_sum += cand_part.numeric_val;
        }
    }
    std::cout << "Part Number Sum: " << example_02_part_num_sum << std::endl;

    std::cout << "########## Day03 Part 1 ###########" << std::endl;

    std::vector<ParsedVal> part_01_vals, part_01_symbols;
    parseInput(puzzle_input_strs, part_01_vals, part_01_symbols);
    int part_01_part_num_sum = 0;
    for (const auto& cand_part : part_01_vals)
    {
        if (has_adjacent_symbol(cand_part, part_01_symbols, puzzle_input_strs[0].size() - 1, puzzle_input_strs.size() - 1))
        {
            part_01_part_num_sum += cand_part.numeric_val;
        }
    }
    std::cout << "Part Number Sum: " << part_01_part_num_sum << std::endl;

    std::cout << "########## Day03 Part 2 ###########" << std::endl;

    std::vector<ParsedVal> part_02_star_symbols;
    for (const auto& symb : part_01_symbols)
    {
        if (symb.str == "*")
        {
            part_02_star_symbols.push_back(symb);
        }
    }

    std::cout << "Found " << part_02_star_symbols.size() << " '*' symbols" << std::endl;

    // for each '*' symbol, find how many have two neighbors, calculate gear ratios
    //  and accumulate
    int part_02_accum_sum = 0;
    for (const auto& symb : part_02_star_symbols)
    {
        std::vector<int> neighbor_vals;
        for (const auto& part : part_01_vals)
        {
            auto cand_symb_locations = part.get_adjacent_locations(puzzle_input_strs[0].size() - 1, puzzle_input_strs.size() - 1);
            if (cand_symb_locations.find(symb.loc) != cand_symb_locations.end())
            {
                neighbor_vals.push_back(part.numeric_val);
            }

            if (neighbor_vals.size() > 2)
            {
                break;
            }
        }

        if (neighbor_vals.size() == 2)
        {
            part_02_accum_sum += neighbor_vals[0] * neighbor_vals[1];
        }
    }
    std::cout << "Calculated accumulated gear ratios " << part_02_accum_sum << std::endl;

    return 0;
}