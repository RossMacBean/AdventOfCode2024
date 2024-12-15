#include <algorithm>
#include <cassert>
#include <format>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

#include "Part2.h"

constexpr uint64_t empty_block_id = std::numeric_limits<std::uint64_t>::max();

std::string read_input(const std::string &filename) {
    std::ifstream file{filename};

    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
    }

    std::string line;
    if (!std::getline(file, line)) {
        std::cerr << "Could not read the line from the file" << std::endl;
        exit(1);
    }

    std::string result(line);
    if (std::getline(file, line)) {
        std::cerr << "More that one line in the file detected, make sure the input is a single line" << std::endl;
        exit(1);
    }

    return result;
}

void print_blocks(const std::vector<uint64_t> &blocks) {
    for (const auto &block : blocks) {
        if (block == empty_block_id) {
            std::cout << '.' << ' ';
        } else {
            std::cout << std::to_string(block) << ' ';;
        }
    }

    std::cout << std::endl;
}

std::vector<uint64_t> read_blocks(const std::string &input) {
    std::vector<uint64_t> result;

    bool is_file = true;
    int64_t file_id = 0;
    for (const auto &c : input) {
        if (c < '0' || c > '9') {
            std::cerr << "Invalid character found in input stream. All values should be 0 - 9" << std::endl;
            exit(1);
        }

        uint64_t block_length = c - '0';
        while (block_length--) {
            result.emplace_back(is_file ? file_id : empty_block_id);
        }

        if (is_file) {
            file_id++;
        }

        is_file = !is_file;
    }

    return result;
}

void reorder_blocks(std::vector<uint64_t> &blocks) {
    size_t front = 0, back = blocks.size() - 1;
    while (front < back) {
        while (blocks[front] != empty_block_id && front < back) {
            front++;
        }

        while (blocks[back] == empty_block_id && front < back) {
            back--;
        }

        const auto tmp = blocks[front];
        blocks[front] = blocks[back];
        blocks[back] = tmp;
    }
}

uint64_t calculate_checksum(const std::vector<uint64_t> &blocks) {
    uint64_t checksum = 0;

    size_t i = 0;
    for (const auto block : blocks) {
        if (block != empty_block_id) {
            checksum += i * blocks[i];
        }
        i++;
    }

    return checksum;
}

uint64_t part1(const std::string &input) {
    auto blocks = read_blocks(input);
    reorder_blocks(blocks);

    return calculate_checksum(blocks);
}

uint64_t part2(const std::string &input) {
    Part2::FileSystem filesystem = Part2::read_input(input);
    filesystem.reorder();

    return filesystem.calculate_checksum();
}

int main() {
    const auto part1_input = read_input("input.txt");
    const size_t part1_result = part1(part1_input);
    std::cout << "Part 1: " << part1_result << std::endl;

    const auto part2_input = read_input("input.txt");
    const size_t part2_result = part2(part2_input);
    std::cout << "Part 2: " << part2_result << std::endl;

    return 0;
}
