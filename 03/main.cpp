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

std::string read_input(const std::string &file_name) {
    std::ifstream file{file_name};

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

constexpr int max_number_digits = 3;
std::optional<int> parse_number(const std::string_view sv) {
    int result;

    if (sv.size() > max_number_digits) {
        return {};
    }

    if (std::from_chars(sv.data(), sv.data() + sv.size(), result).ec == std::errc::invalid_argument) {
        return {};
    }

    return result;
}

// This is crying out for a state machine and some proper encapsulation in an object
const std::string mul_begin_token{"mul("};
const std::string mul_end_token{")"};
std::vector<mul_instruction> parse_mul_instructions(const std::string_view input) {
    std::vector<mul_instruction> result;

    size_t pos = 0;
    for (pos = input.find(mul_begin_token); pos != std::string::npos; pos = input.find(mul_begin_token, ++pos)) {
        int left, right;

        const auto end_pos = input.find(mul_end_token, pos);
        if (end_pos == std::string::npos) continue;

        auto mul_instruction_sv = input.substr(pos, end_pos - pos + 1);

        // Parse first number
        const size_t comma_pos = mul_instruction_sv.find(',');
        if (comma_pos == std::string::npos) continue;

        {
            auto parse_result = parse_number(mul_instruction_sv.substr(mul_begin_token.size(), comma_pos - mul_begin_token.size()));
            if (!parse_result) continue;
            left = parse_result.value();
        }

        // Parse second number
        {
            const auto right_start_pos = comma_pos + 1;
            auto parse_result = parse_number(mul_instruction_sv.substr(right_start_pos, mul_instruction_sv.size() - 1  - right_start_pos));
            if (!parse_result) continue;
            right = parse_result.value();
        }

        result.push_back({ left, right });
    }
    return result;
}

const std::string do_token{"do()"};
const std::string dont_token{"don't()"};
void print_instructions(const std::vector<mul_instruction>& instructions) {
    for (auto [left, right] : instructions) {
        std::cout << left << '*' << right << ' ';
    }
    std::cout << std::endl;
}

int part1(const std::string_view input) {
    int total = 0;

    const auto instructions = parse_mul_instructions(input);
    //print_instructions(instructions);

    for (auto [left, right] : instructions) {
        total += left * right;
    }

    return total;
}

int part2(const std::string_view input) {
    int total = 0;
    bool enabled = true;
    std::vector<mul_instruction> enabled_instructions;

    size_t pos = 0;
    while (pos != std::string::npos) {
        if (enabled) {
            const size_t start_pos = pos;
            size_t end_pos = input.find(dont_token, pos);
            if (end_pos == std::string::npos) {
                end_pos = input.size() - 1;
            }

            const std::string_view enabled_instruction_sv = input.substr(start_pos, end_pos - start_pos);
            auto mul_instructions = parse_mul_instructions(enabled_instruction_sv);
            enabled_instructions.insert(enabled_instructions.end(), mul_instructions.begin(), mul_instructions.end());
            enabled = false;

            pos = end_pos + dont_token.size();
        } else {
            const auto do_pos = input.find(do_token, pos);
            if (do_pos == std::string::npos) {
                break;
            }

            enabled = true;
            pos = do_pos + do_token.size();
        }
    }

    for (auto [left, right] : enabled_instructions) {
        total += left * right;
    }

    return total;
}

int main() {
    //const auto input = read_input("example_input_part1.txt");
    const auto input_part1 = read_input("input.txt");
    const int part1_result = part1(input_part1);
    std::cout << "Part 1: " << part1_result << std::endl;

    const auto input_part2 = read_input("input.txt");
    const int part2_result = part2(input_part2);
    std::cout << "Part 2: " << part2_result << std::endl;

    return 0;
}
