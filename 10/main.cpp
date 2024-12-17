#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

struct Point {
    int32_t row;
    int32_t col;

    bool operator==(const Point &other) const {
        return row == other.row && col == other.col;
    }

    struct HashFunction {
        size_t operator()(const Point& p) const
        {
            // Large primes
            return p.row * 73856093 ^ p.col * 19349663;
        }
    };
};

std::vector<std::vector<uint32_t>> read_input(const std::string &filename) {
    std::ifstream file{filename};

    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
    }

    std::vector<std::vector<uint32_t>> result;
    std::string line;
    while (std::getline(file, line)) {
        std::vector<uint32_t> line_nums;
        line_nums.reserve(line.size());
        for (const auto c : line) {
            if (c < '0' || c > '9') {
                std::cerr << "Bad input detected!" << std::endl;
                exit(1);
            }

            line_nums.push_back(c - '0');
        }
        result.emplace_back(std::move(line_nums));
    }

    return result;
}

bool check_grid_position(const int32_t row, const int32_t col,
    const std::vector<std::vector<uint32_t>> &grid, const uint32_t target_height)
{
    return row >= 0 && row < grid.size() && col >= 0 && col < grid[row].size() && grid[row][col] == target_height;
}

void evaluate_position_part1(
    const int32_t row,
    const int32_t col,
    const std::vector<std::vector<uint32_t>> &grid,
    std::unordered_set<Point, Point::HashFunction> &peaks)
{
    const uint32_t height = grid[row][col];
    if (height == 9) {
        // Found the top
        peaks.insert(Point{.row = row, .col = col});
        return;
    }

    // Up
    if (check_grid_position(row - 1, col, grid, height + 1)) {
        evaluate_position_part1(row - 1, col, grid, peaks);
    }

    // Right
    if (check_grid_position(row, col + 1, grid, height + 1)) {
        evaluate_position_part1(row, col + 1, grid, peaks);
    }

    // Down
    if (check_grid_position(row + 1, col, grid, height + 1)) {
        evaluate_position_part1(row + 1, col, grid, peaks);
    }

    // Left
    if (check_grid_position(row, col - 1, grid, height + 1)) {
        evaluate_position_part1(row, col - 1, grid, peaks);
    }
}

uint64_t part1(const std::vector<std::vector<uint32_t>> &input) {
    uint64_t total = 0;

    for (int32_t row = 0; row < input.size(); row++) {
        for (int32_t col = 0; col < input[row].size(); col++) {
            if (input[row][col] == 0) {
                std::unordered_set<Point, Point::HashFunction> peaks;
                evaluate_position_part1(row, col, input, peaks);
                total += peaks.size();
            }
        }
    }

    std::cout << std::endl;
    return total;
}

void evaluate_position_part2(
    const int32_t row,
    const int32_t col,
    const std::vector<std::vector<uint32_t>> &grid, uint64_t &score)
{
    const uint32_t height = grid[row][col];
    if (height == 9) {
        // Found the top
        score++;
        return;
    }

    // Up
    if (check_grid_position(row - 1, col, grid, height + 1)) {
        evaluate_position_part2(row - 1, col, grid, score);
    }

    // Right
    if (check_grid_position(row, col + 1, grid, height + 1)) {
        evaluate_position_part2(row, col + 1, grid, score);
    }

    // Down
    if (check_grid_position(row + 1, col, grid, height + 1)) {
        evaluate_position_part2(row + 1, col, grid, score);
    }

    // Left
    if (check_grid_position(row, col - 1, grid, height + 1)) {
        evaluate_position_part2(row, col - 1, grid, score);
    }
}

uint64_t part2(const std::vector<std::vector<uint32_t>> &input) {
    uint64_t total = 0;

    for (int32_t row = 0; row < input.size(); row++) {
        for (int32_t col = 0; col < input[row].size(); col++) {
            if (input[row][col] == 0) {
                evaluate_position_part2(row, col, input, total);
            }
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
