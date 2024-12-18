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
};

template <>
struct std::hash<Point> {
    size_t operator()(const Point& p) const noexcept {
        // Large primes
        return p.row * 73856093 ^ p.col * 19349663;
    }
};

struct Direction {
    int8_t row;
    int8_t col;
};
std::array<Direction, 4> cardinal_directions = {{
    {-1, 0},
    {0, 1},
    {1, 0},
    {0, -1}
    }};

struct Region {
    char plant;
    int32_t perimeter;
    int32_t corners;
    std::unordered_set<Point> points;
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

bool in_bounds(const std::vector<std::vector<char>> &grid, const int32_t row, const int32_t col) {
    return row >= 0 && row < grid.size() && col >= 0 && col < grid[row].size();
}

// I have no idea if this is the 'proper' way to check for corners, but it's what I came up with. It looks super janky.
bool corner_check(
    const std::vector<std::vector<char>> &grid,
    const int32_t row,
    const int32_t col,
    const char plant,
    const std::array<Direction, 2> &corner)
{
    /* We have a corner if:
     *      - Both adjacent cardinal points for this corner are either out of bounds, or are not the same plant as us
     *      - If the above is not true, then if the adjacent cardinal points are the same plant as us, AND the point on the diagonal
     *          of this corner is not the same plant as us, then we're an 'inner corner'
    */

    bool both_cardinals_are_different = true;
    for (const auto &direction : corner) {
        const Point p{row + direction.row, col + direction.col};
        both_cardinals_are_different &= !in_bounds(grid, p.row, p.col) || grid[p.row][p.col] != plant;
    }

    if (both_cardinals_are_different) {
        return true;
    }

    bool both_cardinals_are_same = true;
    for (const auto &direction : corner) {
        const Point p{row + direction.row, col + direction.col};
        both_cardinals_are_same &= in_bounds(grid, p.row, p.col) && grid[p.row][p.col] == plant;
    }

    const Point diagonal{row + corner[0].row + corner[1].row, col + corner[0].col + corner[1].col};
    return both_cardinals_are_same && in_bounds(grid, diagonal.row, diagonal.col) && grid[diagonal.row][diagonal.col] != plant;
}

int32_t count_corners(const std::vector<std::vector<char>> &grid, const int32_t row, const int32_t col, const char plant) {
    int32_t result = 0;
    for (int i = 0; i < cardinal_directions.size(); i++) {
        const std::array<Direction, 2> corners = {{cardinal_directions[i], cardinal_directions[(i + 1) % cardinal_directions.size()]}};
        if (corner_check(grid, row, col, plant, corners)) {
            result++;
        }
    }

    return result;
}

Region make_region(const std::vector<std::vector<char>> &grid, const int32_t row, const int32_t col) {
    int32_t perimeter = 0, corners = 0;
    const char plant = grid[row][col];

    std::unordered_set<Point> visited_points;
    std::queue<Point> point_queue;
    point_queue.emplace(row, col);

    while (!point_queue.empty()) {
        const auto [this_row, this_col] = point_queue.front();
        point_queue.pop();

        if (visited_points.contains({this_row, this_col})) {
            // Already visited this point
            continue;
        }

        visited_points.insert({this_row, this_col});
        // Guess who passed row and col in the wrong order and spent an hour debugging it :')
        corners += count_corners(grid, this_row, this_col, plant);

        for (const auto &[dir_row, dir_col] : cardinal_directions) {
            Point p{this_row + dir_row, this_col + dir_col};
            if (in_bounds(grid, p.row, p.col) && grid[p.row][p.col] == plant) {
                point_queue.emplace(p.row, p.col);
            } else {
                perimeter++;
            }
        }
    }

    return {plant, perimeter, corners, std::move(visited_points)};
}

uint64_t part1(const std::vector<std::vector<char>> &input) {
    uint64_t total = 0;
    std::unordered_set<Point> visited_points;

    for (int j = 0; j < input.size(); j++) {
        for (int i = 0; i < input[j].size(); i++) {
            if (visited_points.contains({j, i})) {
                continue;
            }

            Region region = make_region(input, j, i);
            total += region.points.size() * region.perimeter;

            visited_points.insert(region.points.begin(), region.points.end());
        }
    }

    return total;
}

uint64_t part2(const std::vector<std::vector<char>> &input) {
    uint64_t total = 0;
    std::unordered_set<Point> visited_points;

    for (int j = 0; j < input.size(); j++) {
        for (int i = 0; i < input[j].size(); i++) {
            if (visited_points.contains({j, i})) {
                continue;
            }

            Region region = make_region(input, j, i);
            total += region.points.size() * region.corners;

            visited_points.insert(region.points.begin(), region.points.end());
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
