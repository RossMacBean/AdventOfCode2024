#include <format>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <unordered_set>

std::vector<std::vector<char>> read_input(const std::string &filename) {
    std::ifstream file{filename};

    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
    }

    std::string line;
    std::vector<std::vector<char>> result;
    while (std::getline(file, line)) {
        std::vector row(line.begin(), line.end());
        result.emplace_back(row);
    }

    return result;
}

struct Point {
    int32_t x;
    int32_t y;

    bool operator==(const Point &other) const {
        return x == other.x && y == other.y;
    }

    struct HashFunction {
        size_t operator()(const Point& p) const
        {
            // Large primes
            return (p.x * 73856093) ^ (p.y * 19349663);
        }
    };
};

struct Direction {
    int8_t x;
    int8_t y;

    Direction clockwise() const {
        if (x == 0 && y == -1) {
            return Direction{ .x = 1, .y =  0 };
        }

        if (x == 1 && y == 0) {
            return Direction{ .x = 0, .y =  1 };
        }

        if (x == 0 && y == 1) {
            return Direction{ .x = -1, .y =  0 };
        }

        if (x == -1 && y == 0) {
            return Direction{ .x = 0, .y =  -1 };
        }

        // Oops
        std::cerr << "Tried to turn clockwise but the Direction vector wasn't valid" << std::endl;
        return Direction{ .x = 0, .y = 0 }; // Return some default
    }
};

std::optional<Point> find_guard_position(const std::vector<std::vector<char>> & board) {
    for (int32_t y = 0; y < board.size(); y++) {
        for (int32_t x = 0; x < board[y].size(); x++) {
            if (board[y][x] == '^') {
                return Point{ .x = x, .y = y };
            }
        }
    }

    return std::nullopt;
}

bool is_in_bounds(const Point point, const std::vector<std::vector<char>> & board) {
    return point.x >= 0 && point.x < board[point.y].size() && point.y >= 0 && point.y < board.size();
}

size_t part1(const std::vector<std::vector<char>> &board) {
    const auto guard_position_result = find_guard_position(board);
    if ( !guard_position_result.has_value()) {
        std::cerr << "No guard position found" << std::endl;
        return -1;
    }

    Point guard_position = guard_position_result.value();
    std::unordered_set<Point, Point::HashFunction> visited;

    Direction direction{ .x = 0, .y =  -1 };
    bool done = false;
    while (!done) {
        visited.insert(guard_position);

        bool advanced = false;
        while (!advanced) {
            // Advance
            const Point new_guard_position = { guard_position.x + direction.x, guard_position.y + direction.y };

            // Guard has left the area, we're done
            if (!is_in_bounds(new_guard_position, board)) {
                done = true;
                break;
            }

            if (board[new_guard_position.y][new_guard_position.x] == '#') {
                // collision, turn clockwise
                direction = direction.clockwise();
            } else {
                // Clear, move in the direction
                guard_position = new_guard_position;
                advanced = true;
            }
        }
    }

    return visited.size();
}

int part2(const std::vector<std::vector<char>> &board) {
    int32_t total = 0;

    return total;
}

int main() {
    const auto part1_input = read_input("input.txt");
    const size_t part1_result = part1(part1_input);
    std::cout << "Part 1: " << part1_result << std::endl;

    // const auto part2_input = read_input("example_input.txt");
    // const int part2_result = part2(part2_input);
    // std::cout << "Part 2: " << part2_result << std::endl;

    return 0;
}
