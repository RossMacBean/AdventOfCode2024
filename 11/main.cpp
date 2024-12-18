#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <ranges>
#include <string>
#include <vector>

std::vector<uint64_t> read_input(const std::string &filename) {
    std::ifstream file{filename};

    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
    }

    std::vector<uint64_t> result;
    while (!file.eof()) {
        uint64_t val;
        file >> val;
        result.emplace_back(val);
    }

    return result;
}

std::pair<uint64_t, uint64_t> split_stone(const uint64_t stone) {
    const std::string stone_str = std::to_string(stone);
    const size_t middle = stone_str.size() / 2;

    const std::string left_str = stone_str.substr(0, middle);
    const std::string right_str = stone_str.substr(middle);
    return std::make_pair(std::stoull(left_str), std::stoull(right_str));
}

void blink(std::map<uint64_t, int64_t> &stones_map) {
    std::map<uint64_t, int64_t> stones_map_copy = stones_map;
    for (const auto [key, val] : stones_map_copy) {
        if (val == 0) {
            continue;
        }

        if (key == 0) {
            stones_map[1] += val;
        } else if (std::to_string(key).size() % 2 == 0) {
            auto [left, right] = split_stone(key);
            stones_map[left] += val;
            stones_map[right] += val;
        } else {
            stones_map[key * 2024] += val;
        }

        // Remove this stone
        stones_map[key] -= val;
    }
}

uint64_t run(std::vector<uint64_t> &input, int blink_count) {
    // Initialise the stone map
    std::map<uint64_t, int64_t> stone_map;
    for (auto i : input) {
        stone_map[i] = 1;
    }

    while (blink_count--) {
        blink(stone_map);
    }

    // Count the total number of stones
    uint64_t total = 0;
    for (const auto val : std::ranges::views::values(stone_map)) {
        total += val;
    }

    return total;
}

int main() {
    auto part1_input = read_input("input.txt");
    const size_t part1_result = run(part1_input, 25);
    std::cout << "Part 1: " << part1_result << std::endl;

    auto part2_input = read_input("input.txt");
    const size_t part2_result = run(part2_input, 75);
    std::cout << "Part 2: " << part2_result << std::endl;

    return 0;
}
