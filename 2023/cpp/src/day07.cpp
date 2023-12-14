#include <algorithm>
#include <cassert>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "utils.h"

enum PokerHandType {
    UNKNOWN = 0,
    HIGH_CARD,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    FIVE_OF_A_KIND
};

std::string getPokerHandTypeStr(PokerHandType type)
{
    switch (type)
    {
    case PokerHandType::HIGH_CARD: return "HIGH_CARD";
    case PokerHandType::ONE_PAIR: return "ONE_PAIR";
    case PokerHandType::TWO_PAIR: return "TWO_PAIR";
    case PokerHandType::THREE_OF_A_KIND: return "THREE_OF_A_KIND";
    case PokerHandType::FULL_HOUSE: return "FULL_HOUSE";
    case PokerHandType::FOUR_OF_A_KIND: return "FOUR_OF_A_KIND";
    case PokerHandType::FIVE_OF_A_KIND: return "FIVE_OF_A_KIND";
    case PokerHandType::UNKNOWN:
    default:
        return "UNKNOWN";
    }
}

struct PokerHand {
    std::string hand;
    int bid;
    std::map<std::string, int> parsed_hand;
    PokerHandType type;

    bool hasXOfAKind(int x, bool include_j) const {
        for (const auto& iter : parsed_hand)
        {
            if (!include_j && iter.first == "J")
            {
                continue;
            }

            if (iter.second == x)
            {
                return true;
            }
        }
        return false;
    };

    bool hasTwoOfAKind(bool include_j = true) const {
        return hasXOfAKind(2, include_j);
    };

    bool hasThreeOfAKind(bool include_j = true) const {
        return hasXOfAKind(3, include_j);
    };

    bool hasFourOfAKind(bool include_j = true) const {
        return hasXOfAKind(4, include_j);
    };

    bool hasTwoPair(bool include_j = true) const {
        int num_pairs = 0;
        for (const auto& iter : parsed_hand)
        {
            if (!include_j && iter.first == "J")
            {
                continue;
            }

            if (iter.second == 2)
            {
                num_pairs++;
            }
        }
        return num_pairs == 2;
    };

    int getNumJokers(void) const {
        if (parsed_hand.count("J") > 0)
        {
            return parsed_hand.at("J");
        }
        else
        {
            return 0;
        }
    }
};

int getPart01CardVal(char c) {
    const std::map<char, int> char_to_val = {
        {'A', 13},
        {'K', 12},
        {'Q', 11},
        {'J', 10},
        {'T',  9},
        {'9',  8},
        {'8',  7},
        {'7',  6},
        {'6',  5},
        {'5',  4},
        {'4',  3},
        {'3',  2},
        {'2',  1}
    };
    return char_to_val.at(c);
}

int getPart02CardVal(char c) {
    const std::map<char, int> char_to_val = {
        {'A', 13},
        {'K', 12},
        {'Q', 11},
        {'T', 10},
        {'9',  9},
        {'8',  8},
        {'7',  7},
        {'6',  6},
        {'5',  5},
        {'4',  4},
        {'3',  3},
        {'2',  2},
        {'J',  1},
    };
    return char_to_val.at(c);
}

std::map<std::string, int> countCharacterOccurrences(const std::string& str)
{
    std::map<std::string, int> occurrences;

    for (char ch : str)
    {
        std::string key(1, ch); // Convert char to string for map key

        if (occurrences.find(key) != occurrences.end())
        {
            occurrences[key]++;
        }
        else
        {
            occurrences[key] = 1;
        }
    }

    return occurrences;
}

PokerHandType part01_classify_poker_hand(const PokerHand& hand)
{
    if (hand.parsed_hand.size() == 5)
    {
        return PokerHandType::HIGH_CARD;
    }

    if (hand.parsed_hand.size() == 1)
    {
        return PokerHandType::FIVE_OF_A_KIND;
    }

    if (hand.hasFourOfAKind())
    {
        return PokerHandType::FOUR_OF_A_KIND;
    }

    if (hand.hasThreeOfAKind() && hand.hasTwoOfAKind())
    {
        return PokerHandType::FULL_HOUSE;
    }

    if (hand.hasThreeOfAKind())
    {
        return PokerHandType::THREE_OF_A_KIND;
    }

    if (hand.hasTwoPair())
    {
        return PokerHandType::TWO_PAIR;
    }

    if (hand.hasTwoOfAKind())
    {
        return PokerHandType::ONE_PAIR;
    }

    assert(false);
}


PokerHandType part02_classify_poker_hand(const PokerHand& hand)
{
    if (hand.getNumJokers() == 0 || hand.getNumJokers() == 5)
    {
        return part01_classify_poker_hand(hand);
    }

    PokerHand new_hand;
    new_hand.bid = hand.bid;
    new_hand.parsed_hand = hand.parsed_hand;

    std::string most_common_non_joker_card = "-";
    int most_common_non_joker_card_occurrences = 0;
    for (const auto& card : hand.parsed_hand)
    {
        if (card.first == "J")
        {
            continue;
        }
        if (card.second > most_common_non_joker_card_occurrences)
        {
            most_common_non_joker_card = card.first;
            most_common_non_joker_card_occurrences = card.second;
        }
    }
    assert(most_common_non_joker_card != "-");
    assert(most_common_non_joker_card_occurrences > 0);

    new_hand.parsed_hand[most_common_non_joker_card] = new_hand.parsed_hand[most_common_non_joker_card] + hand.getNumJokers();
    new_hand.parsed_hand.erase("J");

    return part01_classify_poker_hand(new_hand);
}

std::vector<PokerHand> parsePokerHands(const std::string& filename, bool part2)
{
    std::vector<PokerHand> hands;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Unable to open file (" << filename << ")" << std::endl;
        return hands; // Returning empty vector on file open failure
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string hand;
        int bid;

        if (iss >> hand >> bid)
        {
            auto parsed_hand = countCharacterOccurrences(hand);
            auto next_hand = PokerHand{
                .hand = hand,
                .bid = bid,
                .parsed_hand = parsed_hand,
                .type = PokerHandType::UNKNOWN,
            };
            PokerHandType hand_type = PokerHandType::UNKNOWN;
            if (!part2)
            {
                hand_type = part01_classify_poker_hand(next_hand);
            }
            else
            {
                hand_type = part02_classify_poker_hand(next_hand);
            }
            next_hand.type = hand_type;
            hands.push_back(next_hand);
        }
        else
        {
            std::cerr << "Error parsing line: " << line << std::endl;
        }
    }

    file.close();
    return hands;
}

// Comparison function for sorting PokerHand objects based on the "hand" string values
bool part01_comparePokerHands(const PokerHand& a, const PokerHand& b) {
    if (a.type == b.type)
    {
        for (int ii = 0; ii < a.hand.size() && ii < b.hand.size(); ++ii)
        {
            if (getPart01CardVal(a.hand[ii]) != getPart01CardVal(b.hand[ii]))
            {
                return getPart01CardVal(a.hand[ii]) > getPart01CardVal(b.hand[ii]);
            }
        }
        assert(false);
    }
    else
    {
        return a.type > b.type;
    }
}

bool part02_comparePokerHands(const PokerHand& a, const PokerHand& b) {
    if (a.type == b.type)
    {
        for (int ii = 0; ii < a.hand.size() && ii < b.hand.size(); ++ii)
        {
            if (getPart02CardVal(a.hand[ii]) != getPart02CardVal(b.hand[ii]))
            {
                return getPart02CardVal(a.hand[ii]) > getPart02CardVal(b.hand[ii]);
            }
        }
        assert(false);
    }
    else
    {
        return a.type > b.type;
    }
}

int calculate_total_winnings(const std::vector<PokerHand> hands)
{
    int ret = 0;

    for (int idx = 0; idx < hands.size(); ++idx)
    {
        int rank = hands.size() - idx;
        ret += rank * hands[idx].bid;
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

    std::string example01_data_file_path = options.dataDir + "/day07/example01.txt";
    std::string example02_data_file_path = options.dataDir + "/day07/example02.txt";
    std::string puzzle_data_file_path = options.dataDir + "/day07/puzzle.txt";

    std::cout << "########## Day07 Example 01 ##########" << std::endl;

    std::vector<PokerHand> example01_parsed_hands = parsePokerHands(example01_data_file_path, false);

    // Sorting the vector based on hand string values
    std::sort(example01_parsed_hands.begin(), example01_parsed_hands.end(), part01_comparePokerHands);

    // Displaying parsed hands
    for (const auto& hand : example01_parsed_hands) {
        std::cout << "Hand: " << hand.hand << ", Bid: " << hand.bid << " Type: " << hand.type << std::endl;
    }

    std::cout << "Accum Winnings: " << calculate_total_winnings(example01_parsed_hands) << std::endl;

    std::cout << "########## Day07 Part 01 #############" << std::endl;

    std::vector<PokerHand> part01_parsed_hands = parsePokerHands(puzzle_data_file_path, false);

    // Sorting the vector based on hand string values
    std::sort(part01_parsed_hands.begin(), part01_parsed_hands.end(), part01_comparePokerHands);

    std::cout << "Accum Winnings: " << calculate_total_winnings(part01_parsed_hands) << std::endl;

    std::cout << "########## Day07 Example 02 ##########" << std::endl;

    example01_parsed_hands = parsePokerHands(example01_data_file_path, true);
    std::vector<PokerHand> example02_parsed_hands = parsePokerHands(example02_data_file_path, true);

    // Sorting the vector based on hand string values
    std::sort(example01_parsed_hands.begin(), example01_parsed_hands.end(), part02_comparePokerHands);
    std::sort(example02_parsed_hands.begin(), example02_parsed_hands.end(), part02_comparePokerHands);

    // Displaying parsed hands
    for (const auto& hand : example02_parsed_hands) {
        std::cout << "Hand: " << hand.hand << " Type: " << getPokerHandTypeStr(hand.type) << std::endl;
    }

    std::cout << "Accum Winnings (Part01): " << calculate_total_winnings(example01_parsed_hands) << std::endl;

    std::cout << "########## Day07 Part 02 #############" << std::endl;

    std::vector<PokerHand> part02_parsed_hands = parsePokerHands(puzzle_data_file_path, true);

    // Sorting the vector based on hand string values
    std::sort(part02_parsed_hands.begin(), part02_parsed_hands.end(), part02_comparePokerHands);

    // Displaying parsed hands
    for (const auto& hand : part02_parsed_hands) {
            std::cout << "Hand: " << hand.hand << " Type: " << getPokerHandTypeStr(hand.type) << std::endl;
    }

    std::cout << "Accum Winnings: " << calculate_total_winnings(part02_parsed_hands) << std::endl;

    return 0;
}
