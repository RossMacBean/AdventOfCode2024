#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Equation {
    int32_t a1;
    int32_t a2;
    int32_t b1;
    int32_t b2;
    int64_t c1;
    int64_t c2;
};

std::pair<int32_t, int32_t> read_line(const std::string &line) {
    const size_t x_pos = line.find('X');
    int32_t x = std::stoi(line.substr(x_pos + 2, line.find(',', x_pos) - x_pos));

    int32_t y = std::stoi(line.substr(line.find('Y') + 2));

    return {x, y};
}

std::vector<Equation> read_input(const std::string &filename) {
    std::ifstream file{filename};

    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
    }

    std::vector<Equation> result;
    while (!file.eof()) {
        std::string line;
        std::getline(file, line);
        auto [a1, a2] = read_line(line);

        std::getline(file, line);
        auto [b1, b2] = read_line(line);

        std::getline(file, line);
        auto [c1, c2] = read_line(line);

        result.emplace_back(a1, a2, b1, b2, c1, c2);

        std::getline(file, line);
    }

    return result;
}

// I've not done linear algebra for 15 years...
double_t find_x(const Equation &eq) {
    /*
    * The formula is:
    * x = b2c1 - b1c2 / b2a1 - b1a2
    */
    return static_cast<double>(eq.b2 * eq.c1 - eq.b1 * eq.c2) / static_cast<double>(eq.b2 * eq.a1 - eq.b1 * eq.a2);
}

double_t solve_for_y(const Equation &eq, const double_t x) {
    // Substitute X into the Y equation
    return (eq.c1 - eq.a1 * x) / eq.b1;
}

uint64_t part1(const std::vector<Equation> &input) {
    uint64_t total = 0;

    for (const auto eq : input) {
        const double_t x = find_x(eq);
        const double_t y = solve_for_y(eq, x);

        // Only care about prizes we can actually win
        if (std::floor(x) == x && std::floor(y) == y) {
            // Cost function is 3x+y
            const double_t cost = 3 * x + y;
            total += cost;
        }
    }

    return total;
}

uint64_t part2(const std::vector<Equation> &input) {
    uint64_t total = 0;

    for (auto eq : input) {
        eq.c1 += 10000000000000;
        eq.c2 += 10000000000000;
        const double_t x = find_x(eq);
        const double_t y = solve_for_y(eq, x);

        // Only care about prizes we can actually win
        if (std::floor(x) == x && std::floor(y) == y) {
            // Cost function is 3x+y
            const double_t cost = 3 * x + y;
            total += cost;
        }
    }

    return total;
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
