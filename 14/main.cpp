#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
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

    void tick();
};

Vector2 parse_vector2(const std::string &s) {
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

    int32_t rows, cols;
    std::vector<Robot> robots;
    file >> cols >> rows;

    std::string line;
    std::getline(file, line); // Consume the rest of the first line
    while (std::getline(file, line)) {
        const size_t space_pos = line.find(' ');
        Vector2 pos = parse_vector2(line.substr(0, space_pos));
        Vector2 vel = parse_vector2(line.substr(space_pos + 1));

        robots.emplace_back(pos, vel);
    }

    return {rows, cols, std::move(robots)};
}

Vector2 add_vector_with_world_wrapping(const Vector2 a, const Vector2 b, const World &world) {
    int32_t x = (a.x + b.x) % world.cols;
    if (x < 0) {
        x = world.cols + x;
    }

    int32_t y = (a.y + b.y) % world.rows;
    if (y < 0) {
        y = world.rows + y;
    }

    return {x, y};
}

void tick(World &world) {
    for (Robot &robot : world.robots) {
        robot.position = add_vector_with_world_wrapping(robot.position, robot.velocity, world);
    }
}

std::unordered_map<Vector2, int32_t> build_world_point_to_robot_count_map(const World &world) {
    std::unordered_map<Vector2, int32_t> world_point_to_robot_count_map;

    for (const Robot &robot : world.robots) {
        world_point_to_robot_count_map[robot.position]++;
    }

    return std::move(world_point_to_robot_count_map);
}

uint64_t calculate_safety_factor(const World &world, const std::unordered_map<Vector2, int32_t> &point_to_robot_count_map) {
    std::array<int32_t, 4> quadrants = {};
    const int32_t row_mid_point = world.rows / 2;
    const int32_t col_mid_point = world.cols / 2;
    for (const auto [pos, count] : point_to_robot_count_map) {
        if (pos.x < col_mid_point && pos.y < row_mid_point) {
            quadrants[0] += count;
        } else if (pos.x > col_mid_point && pos.y < row_mid_point) {
            quadrants[1] += count;
        }else if (pos.x < col_mid_point && pos.y > row_mid_point) {
            quadrants[2] += count;
        } else if (pos.x > col_mid_point && pos.y > row_mid_point) {
            quadrants[3] += count;
        }
    }

    return quadrants[0] * quadrants[1] * quadrants[2] * quadrants[3];
}

void print_world(const World &world, const std::unordered_map<Vector2, int32_t> &point_to_robot_count_map) {
    for (int y = 0; y < world.rows; y++) {
        for (int x = 0; x < world.cols; x++) {
            if (point_to_robot_count_map.contains({x, y})) {
                std::cout << point_to_robot_count_map.at({x, y});
            } else {
                std::cout << '.';
            }
            std::cout << ' ';
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

    const std::unordered_map<Vector2, int32_t> world_point_to_robot_count_map = build_world_point_to_robot_count_map(world);
    //print_world(world, world_point_to_robot_count_map);

    return calculate_safety_factor(world, world_point_to_robot_count_map);
}

uint64_t part2(const World &world) {
    uint64_t total = 0;


    return total;
}

int main() {
    auto part1_input = read_input("input.txt");
    const size_t part1_result = part1(part1_input);
    std::cout << "Part 1: " << part1_result << std::endl;

    // const auto part2_input = read_input("input.txt");
    // const size_t part2_result = part2(part2_input);
    // std::cout << "Part 2: " << part2_result << std::endl;

    return 0;
}
