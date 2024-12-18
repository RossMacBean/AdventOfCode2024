#include <cstdint>
#include <fstream>
#include <iostream>
#include <queue>
#include <ranges>
#include <string>
#include <unordered_set>
#include <vector>

// Who knew there would be so many grid based puzzles... I keep copy-pasting this struct, maybe I should
// put it somewhere shared...
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

struct Region {
    char plant;
    int32_t area;
    int32_t perimeter;
    std::unordered_set<Point, Point::HashFunction> points;
};

std::vector<std::vector<char>> read_input(const std::string &filename) {
    std::ifstream file{filename};

    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
    }

    std::vector<std::vector<char>> result;
    std::string line;
    while (std::getline(file, line)) {
        result.emplace_back(line.begin(), line.end());
    }

    return result;
}

bool bounds_check(const std::vector<std::vector<char>> &input, const int32_t row, const int32_t col) {
    return row >= 0 && row < input.size() && col >= 0 && col < input[row].size();
}

Region make_region(const std::vector<std::vector<char>> &grid, const int32_t row, const int32_t col) {
    int32_t area = 0, perimeter = 0;
    const char plant = grid[row][col];
    std::unordered_set<Point, Point::HashFunction> points;

    std::queue<Point> point_queue;
    point_queue.emplace(row, col);
    while (!point_queue.empty()) {
        const auto [this_row, this_col] = point_queue.front();
        point_queue.pop();

        // Already visited this point
        if (points.contains({this_row, this_col})) {
            continue;
        }

        points.insert({this_row, this_col});
        area++;

        std::array<Point, 4> neighbours = {{
            {this_row - 1, this_col},
            {this_row, this_col + 1},
            {this_row + 1, this_col},
            {this_row, this_col - 1}
        }};

        for (const auto &[neighbour_row, neighbour_col] : neighbours) {
            if (bounds_check(grid, neighbour_row, neighbour_col) && grid[neighbour_row][neighbour_col] == plant) {
                point_queue.emplace(neighbour_row, neighbour_col);
            } else {
                perimeter++;
            }
        }
    }

    return {plant, area, perimeter, std::move(points)};
}

uint64_t part1(const std::vector<std::vector<char>> &input) {
    uint64_t total = 0;
    std::unordered_set<Point, Point::HashFunction> visited_points;

    for (int j = 0; j < input.size(); j++) {
        for (int i = 0; i < input[j].size(); i++) {
            if (visited_points.contains({j, i})) {
                continue;
            }

            Region region = make_region(input, j, i);
            total += region.area * region.perimeter;

            visited_points.insert(region.points.begin(), region.points.end());
        }
    }

    return total;
}

uint64_t part2(const std::vector<std::vector<char>> &input) {
    uint64_t total = 0;
    std::unordered_set<Point, Point::HashFunction> visited_points;

    for (int j = 0; j < input.size(); j++) {
        for (int i = 0; i < input[j].size(); i++) {
            if (visited_points.contains({j, i})) {
                continue;
            }

            Region region = make_region(input, j, i);
            total += region.area * region.perimeter;

            visited_points.insert(region.points.begin(), region.points.end());
        }
    }

    return total;
}

int main() {
    const auto part1_input = read_input("input.txt");
    const size_t part1_result = part1(part1_input);
    std::cout << "Part 1: " << part1_result << std::endl;

    const auto part2_input = read_input("example_input.txt");
    const size_t part2_result = part2(part2_input);
    std::cout << "Part 2: " << part2_result << std::endl;

    return 0;
}
