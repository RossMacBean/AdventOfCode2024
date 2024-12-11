#include <assert.h>
#include <format>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <unordered_set>

struct Equation {
    int64_t test_value;
    std::vector<int32_t> numbers;
};

std::vector<Equation> read_input(const std::string &filename) {
    std::ifstream file{filename};

    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
    }

    std::vector<Equation> result;
    std::string line;
    while (std::getline(file, line)) {
        const auto colon_pos = line.find(':');
        const int64_t test_value = std::stoll(line.substr(0, colon_pos));
        line.erase(0, colon_pos + 2); // Erase the space after the colon too

        std::vector<int32_t> numbers;
        size_t space_pos;
        while ((space_pos = line.find(' ')) != std::string::npos) {
            numbers.push_back(std::stoi(line.substr(0, space_pos)));
            line.erase(0, space_pos + 1);
        }
        numbers.push_back(std::stoi(line));

        result.emplace_back(test_value, numbers);
    }

    return result;
}

int64_t concat_numbers(const int64_t a, const int64_t b) {
     return std::stoll(std::to_string(a) + std::to_string(b));
}

int64_t evaluate_equation(const std::vector<int32_t> &numbers, const std::vector<char> &operators) {
    assert(numbers.size() == operators.size() + 1);

    int64_t total = numbers[0];
    for (int i = 0; i < operators.size(); i++) {
        const int32_t a = numbers[i + 1];

        switch (operators[i]) {
            case '+':
                total += a;
            break;
            case '*':
                total *= a;
            break;
            case '|':
                total = concat_numbers(total, a);
            break;
            default:
                assert(false);
        }
    }

    return total;
}

bool find_solution(
    const Equation &equation,
    const int i,
    const std::vector<char> &allowed_operators,
    std::vector<char> &current_operators)
{
    // Base case
    if (i == equation.numbers.size() - 1) {
        const auto total = evaluate_equation(equation.numbers, current_operators);
        return total == equation.test_value;
    }

    for (auto c : allowed_operators) {
        current_operators.push_back(c);
        if (find_solution(equation, i + 1, allowed_operators, current_operators)) {
            return true;
        }
        current_operators.pop_back();
    }

    return false;
}

int64_t part1(const std::vector<Equation> &equations) {
    int64_t total = 0;

    for (const auto& eq : equations) {
        std::vector<char> operators;
        if (find_solution(eq, 0, {'+', '*'}, operators)) {
            total += eq.test_value;
        }
    }

    return total;
}


int64_t part2(const std::vector<Equation> &equations) {
    int64_t total = 0;

    for (const auto& eq : equations) {
        std::vector<char> operators;
        if (find_solution(eq, 0, {'+', '*', '|'}, operators)) {
            total += eq.test_value;
        }
    }

    return total;
}

int main() {
    const auto part1_input = read_input("input.txt");
    const size_t part1_result = part1(part1_input);
    std::cout << "Part 1: " << part1_result << std::endl;

    auto part2_input = read_input("input.txt");
    const size_t part2_result = part2(part2_input);
    std::cout << "Part 2: " << part2_result << std::endl;

    return 0;
}
