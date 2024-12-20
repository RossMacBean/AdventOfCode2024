#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

#include "../shared/Vector2.h"

struct Robot {
    Vector2 position;
    Vector2 velocity;
};

struct World {
    int32_t rows;
    int32_t cols;
    std::vector<Robot> robots;
};

typedef std::vector<std::vector<int32_t>> Grid;

Vector2 vector2_parse(const std::string &s) {
    const size_t eq_pos = s.find('=');
    const size_t comma_pos = s.find(',');
    const int32_t x = std::stoi(s.substr(eq_pos + 1, comma_pos - eq_pos));
    const int32_t y = std::stoi(s.substr(comma_pos + 1));

    return {x, y};
}

// I've modified the input so the first line contains the width and height of the world so that
// I don't have to hardcode it and change it manually when switching between the example input and the real input
World read_input(const std::string &filename) {
    std::ifstream file{filename};

    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
        exit(1);
    }

    std::vector<Robot> robots;
    int32_t rows, cols;
    file >> cols >> rows;

    std::string line;
    std::getline(file, line); // Consume the rest of the first line
    while (std::getline(file, line)) {
        const size_t space_pos = line.find(' ');
        Vector2 pos = vector2_parse(line.substr(0, space_pos));
        Vector2 vel = vector2_parse(line.substr(space_pos + 1));

        robots.emplace_back(pos, vel);
    }

    return {rows, cols, std::move(robots)};
}

Vector2 vector2_add_with_world_wrapping(const Vector2 a, const Vector2 b, const World &world) {
    int32_t x = (a.x + b.x) % world.cols;
    if (x < 0) {
        x += world.cols;
    }

    int32_t y = (a.y + b.y) % world.rows;
    if (y < 0) {
        y += world.rows;
    }

    return {x, y};
}

void tick(World &world) {
    for (Robot &robot : world.robots) {
        robot.position = vector2_add_with_world_wrapping(robot.position, robot.velocity, world);
    }
}

std::unordered_map<Vector2, int32_t> world_point_to_robot_count_map_create(const World &world) {
    std::unordered_map<Vector2, int32_t> world_point_to_robot_count_map;

    for (const Robot &robot : world.robots) {
        world_point_to_robot_count_map[robot.position]++;
    }

    return std::move(world_point_to_robot_count_map);
}

uint64_t calculate_safety_factor(const World &world, const std::unordered_map<Vector2, int32_t> &point_to_robot_count_map) {
    std::array<int32_t, 4> quadrants{};
    const int32_t row_mid_point = world.rows / 2;
    const int32_t col_mid_point = world.cols / 2;
    for (const auto [pos, count] : point_to_robot_count_map) {
        if (pos.x < col_mid_point && pos.y < row_mid_point) {
            quadrants[0] += count;
        } else if (pos.x > col_mid_point && pos.y < row_mid_point) {
            quadrants[1] += count;
        } else if (pos.x < col_mid_point && pos.y > row_mid_point) {
            quadrants[2] += count;
        } else if (pos.x > col_mid_point && pos.y > row_mid_point) {
            quadrants[3] += count;
        }
    }

    return quadrants[0] * quadrants[1] * quadrants[2] * quadrants[3];
}

Grid world_grid_create(const World &world, const std::unordered_map<Vector2, int32_t> &point_to_robot_count_map) {
    std::vector grid(world.rows, std::vector<int32_t>(world.cols));

    for (int y = 0; y < world.rows; y++) {
        for (int x = 0; x < world.cols; x++) {
            if (point_to_robot_count_map.contains({x, y})) {
                grid[y][x] += point_to_robot_count_map.at({x, y});
            }
        }
    }

    return grid;
}

void world_grid_print(const Grid &grid) {
    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[0].size(); x++) {
            if (grid[y][x] == 0) {
                std::cout << '.';
            } else {
                std::cout << grid[y][x];
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


uint64_t part1(World &world) {
    int32_t ticks = 100;
    while (ticks--) {
        tick(world);
    }

    const auto map = world_point_to_robot_count_map_create(world);
    return calculate_safety_factor(world, map);
}

// It just so happens that for my input that the first tick where no robot overlaps is the first tick where
// the Christmas tree appears. I'm still not sure what the "correct" way of solving this puzzle is, this feels more like
// exploiting how the puzzle was created (start with a tree and then mix it up backwards) and it happens that the grid
// the designer started with has no overlaps? Is that part of the puzzle or did I just get lucky? I don't know. Very unsatisfying puzzle.
// I tried a bunch of different things and this one just happened to work.
void part2(World &world) {
    bool quit = false;
    int tick_count = 0;

    // Keep iterating until we find a position where no robots overlap
    bool found = false;
    while (!found) {
        tick(world);
        tick_count++;

        const auto world_point_to_robot_count_map = world_point_to_robot_count_map_create(world);

        found = true;
        for (const auto count : std::views::values(world_point_to_robot_count_map)) {
            if (count > 1) {
                found = false;
                break;
            }
        }
    }

    std::cout << "Found a tick where no robots overlap" <<std::endl;
    while (!quit) {
        const auto world_point_to_robot_count_map = world_point_to_robot_count_map_create(world);
        const auto grid = world_grid_create(world, world_point_to_robot_count_map);
        std::cout << "Tick: " << tick_count << std::endl;
        world_grid_print(grid);

        char key;
        std::cin >> key;
        if (key == 'q') {
            quit = true;
        } else {
            tick(world);
        }
    }
}

int main() {
    auto part1_input = read_input("input.txt");
    const size_t part1_result = part1(part1_input);
    std::cout << "Part 1: " << part1_result << std::endl;

    auto part2_input = read_input("input.txt");
    part2(part2_input);

    return 0;
}
