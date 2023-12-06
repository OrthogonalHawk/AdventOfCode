#include <bits/stdc++.h>
#include <chrono>
#include <iostream>
#include <fstream>
#include <limits>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "utils.h"

struct ConversionEntry {
    int64_t dst_start;
    int64_t src_start;
    int64_t size;
};

struct ConversionMap {
    std::string map_name;
    std::vector<ConversionEntry> entries;

    int64_t convert_to_dst(int64_t src) const
    {
        // find any ranges that include the src
        for (const auto& entry : entries)
        {
            if (src >= entry.src_start && src < (entry.src_start + entry.size))
            {
                return entry.dst_start + (src - entry.src_start);
            }
        }

        // if no value range, then it's a straight conversion
        return src;
    }
};

struct Almanac {
    std::vector<ConversionMap> maps;

    int64_t convert_to_dst(int64_t src) const
    {
        int64_t cur_val = src;
        for (const auto& map : maps)
        {
            cur_val = map.convert_to_dst(cur_val);
        }

        return cur_val;
    }

    void print_maps(void) const
    {
        for (const auto& map : maps) {
            std::cout << "Map Name: " << map.map_name << std::endl;
            for (const auto& entry : map.entries) {
                std::cout << "{ " << entry.dst_start << ", " << entry.src_start << ", " << entry.size << " }" << std::endl;
            }
            std::cout << std::endl;
        }
    }
};

Almanac parseInputFile(const std::string& filename) {
    std::ifstream inputFile(filename);
    Almanac result;
    std::string line;

    while (std::getline(inputFile, line)) {
        if (line.empty() || line.find("seeds:") != std::string::npos) {
            continue; // Skip empty lines or lines containing "seeds:"
        }

        if (line.back() == ':') {
            // Extract map name
            std::string mapName = line.substr(0, line.size() - 1);
            ConversionMap map;
            map.map_name = mapName;

            std::vector<ConversionEntry> entries;
            while (std::getline(inputFile, line) && !line.empty()) {
                std::istringstream iss(line);
                int64_t dstStart, srcStart, size;
                iss >> dstStart >> srcStart >> size;
                entries.push_back({ dstStart, srcStart, size });
            }

            map.entries = entries;
            result.maps.push_back(map);
        }
    }

    return result;
}

std::vector<int64_t> parseIntegersAfterSeeds(const std::string& filename) {
    std::ifstream inputFile(filename);
    std::vector<int64_t> result;
    std::string line;

    while (std::getline(inputFile, line)) {
        size_t found = line.find("seeds:");
        if (found != std::string::npos) {
            std::istringstream iss(line.substr(found + 6)); // Skip "seeds: "
            int64_t num;
            while (iss >> num)
            {
                result.push_back(num);
            }
        }
    }

    return result;
}

struct SeedPair {
    int64_t start_seed = -1;
    int64_t size = -1;
};

std::vector<SeedPair> parseSeedPairsAfterSeeds(const std::string& filename) {
    std::ifstream inputFile(filename);
    std::vector<SeedPair> result;
    std::string line;

    while (std::getline(inputFile, line)) {
        size_t found = line.find("seeds:");
        if (found != std::string::npos) {
            std::istringstream iss(line.substr(found + 6)); // Skip "seeds: "
            int64_t num1, num2;
            while (iss >> num1 >> num2)
            {
                result.push_back(SeedPair{num1, num2});
            }
        }
    }

    return result;
}

void find_smallest_seed_location(const SeedPair& seed_info, const Almanac& almanac, std::vector<int64_t>& resultQueue, std::mutex& resultMutex)
{
    int64_t smallest_loc = std::numeric_limits<int64_t>::max();

    int64_t num_seeds_checked = 0;
    for (int64_t seed = seed_info.start_seed; seed < (seed_info.start_seed + seed_info.size); ++seed)
    {
        int64_t next_loc = almanac.convert_to_dst(seed);
        smallest_loc = std::min(smallest_loc, next_loc);

        num_seeds_checked++;
    }

    // Push result to the resultQueue (thread-safe)
    std::lock_guard<std::mutex> lock(resultMutex);
    resultQueue.push_back(smallest_loc);
}

int main(int argc, char *argv[])
{
    CommandLineOptions options = parse_command_line(argc, argv);
    if (options.helpRequested)
    {
        print_command_line_usage(argv);
        return 1;
    }

    std::string example01_data_file_path = options.dataDir + "/day05/example01.txt";
    std::string puzzle_data_file_path = options.dataDir + "/day05/puzzle.txt";

    std::cout << "########## Day05 Example 01 ##########" << std::endl;

    auto example01_almanac = parseInputFile(example01_data_file_path);
    std::vector<int64_t> example01_seeds = parseIntegersAfterSeeds(example01_data_file_path);

    int64_t example01_min_location = std::numeric_limits<int64_t>::max();
    for (const auto& seed : example01_seeds)
    {
        auto next_loc = example01_almanac.convert_to_dst(seed);
        example01_min_location = std::min(example01_min_location, next_loc);
    }

    std::cout << "Min Location: " << example01_min_location << std::endl;

    std::cout << "########## Day05 Part 01 #############" << std::endl;

    auto part01_almanac = parseInputFile(puzzle_data_file_path);
    std::vector<int64_t> part01_seeds = parseIntegersAfterSeeds(puzzle_data_file_path);

    int64_t part01_min_location = std::numeric_limits<int64_t>::max();
    for (const auto& seed : part01_seeds)
    {
        int64_t next_loc = part01_almanac.convert_to_dst(seed);
        part01_min_location = std::min(part01_min_location, next_loc);
    }
    std::cout << "Min Location: " << part01_min_location << std::endl;

    std::cout << "########## Day05 Part 02 #############" << std::endl;

    auto part02_almanac = parseInputFile(puzzle_data_file_path);
    auto part02_seed_pairs = parseSeedPairsAfterSeeds(puzzle_data_file_path);

    std::cout << "Found " << part02_seed_pairs.size() << " seed pairs to check" << std::endl;

    auto part02_start = std::chrono::high_resolution_clock::now();

    constexpr size_t num_threads = 18;
    std::vector<std::thread> threads;
    std::vector<int64_t> resultQueue;
    std::mutex resultMutex;

    for (const auto& seed_pair : part02_seed_pairs)
    {
        int64_t num_seeds_per_thread = seed_pair.size / num_threads;

        for (int ii = 0; ii < num_threads; ++ii)
        {
            auto next_seed_pair = SeedPair{seed_pair.start_seed + ii * num_seeds_per_thread, num_seeds_per_thread};
            if ((ii + 1 == num_threads))
            {
                next_seed_pair = SeedPair{seed_pair.start_seed + ii * num_seeds_per_thread, num_seeds_per_thread + (int64_t)(seed_pair.size % num_threads)};
            }

            threads.emplace_back([next_seed_pair, part02_almanac, &resultQueue, &resultMutex]() {
                find_smallest_seed_location(next_seed_pair, part02_almanac, resultQueue, resultMutex);
            });
        }

        // Wait for all threads to finish their work
        for (auto& thread : threads) {
            thread.join();
        }
        threads.clear();

        std::cout << "Finished seed pair... checked " << seed_pair.size << " seeds" << std::endl;
    }

    auto part02_end = std::chrono::high_resolution_clock::now();
    auto part02_duration = std::chrono::duration_cast<std::chrono::milliseconds>(part02_end - part02_start);

    std::cout << "Min Location: " << *min_element(resultQueue.begin(), resultQueue.end()) << " found after " << part02_duration.count() << " ms" << std::endl;

    return 0;
}
