#include <cmath>
#include <cassert>
#include <format>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct mul_instruction {
    int left;
    int right;
};

std::string read_input() {
    //std::ifstream file{"example_input.txt"};
    std::ifstream file{"input.txt"};

    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
    }

    std::string result;

    std::string line;
    while (std::getline(file, line)) {
        result += line;
    }

    return result;
}

const std::string mul_operator{"mul"};
std::vector<mul_instruction> parse_instructions(const std::string& input) {
    std::vector<mul_instruction> result;

    size_t pos = 0;
    for (pos = input.find(mul_operator); pos != std::string::npos; pos = input.find(mul_operator, pos)) {
        int left, right;

        pos += mul_operator.length();
        if (pos >= input.length()) continue;
        if (input[pos++] != '(') continue;

        // Parse first number
        {
            size_t digit_start_pos = pos;
            size_t digit_end_pos = input.find(',', digit_start_pos);
            size_t num_digits = digit_end_pos - digit_start_pos;

            if (num_digits > 3) continue;
            std::string s = input.substr(digit_start_pos, num_digits);
            if (std::stringstream ss{s}; !(ss >> left)) continue;
            pos = digit_end_pos + 1;
        }

        // Parse second number
        {
            size_t digit_start_pos = pos;
            size_t digit_end_pos = input.find(')', digit_start_pos);
            size_t num_digits = digit_end_pos - digit_start_pos;

            if (num_digits > 3) continue;
            std::string s = input.substr(digit_start_pos, num_digits);
            if (std::stringstream ss{s}; !(ss >> right)) continue;
            pos = digit_end_pos + 1;
        }

        result.push_back({ left, right });
    }
    return result;
}

int part1(const std::vector<mul_instruction>& instructions) {
    int total = 0;

    for (auto [left, right] : instructions) {
        total += left * right;
    }

    return total;
}

int part2(const std::vector<std::vector<int>>& reports) {
    return 0;
}

int main() {
    auto input = read_input();
    auto instructions = parse_instructions(input);

    for (auto [left, right] : instructions) {
        std::cout << left << '*' << right << ' ';
    }
    std::cout << std::endl;

    const int part1_result = part1(instructions);
    std::cout << "Part 1: " << part1_result << std::endl;
    //
    // int part2_result = part2(input);
    // std::cout << "Part 2 Simple: " << part2_result << std::endl;


    return 0;
}
