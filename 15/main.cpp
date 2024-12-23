#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../shared/Direction.h"
#include "../shared/Vector2.h"
#include "../shared/Grid2.h"

enum class Entity {
    EmptySpace,
    Wall,
    Box,
    Robot
};

std::pair<Grid2<Entity>, std::vector<Vector2>> read_input(const std::string &filename) {
    std::ifstream file{filename};

    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
        exit(1);
    }

    std::vector<std::vector<Entity>> cells;
    std::string line;
    while (std::getline(file, line) && !line.empty()) {
        std::vector<Entity> row;
        row.reserve(line.size());

        for (const auto &c : line) {
            switch (c) {
                case '#': row.emplace_back(Entity::Wall); break;
                case '.': row.emplace_back(Entity::EmptySpace); break;
                case 'O': row.emplace_back(Entity::Box); break;
                case '@': row.emplace_back(Entity::Robot); break;
                default: assert(false);
            }
        }

        cells.emplace_back(std::move(row));
    }

    std::vector<Vector2> directions;
    while (std::getline(file, line)) {
        for (const auto &c : line) {
            switch (c) {
                case '^': directions.emplace_back(Direction::Up); break;
                case '>': directions.emplace_back(Direction::Right); break;
                case 'v': directions.emplace_back(Direction::Down); break;
                case '<': directions.emplace_back(Direction::Left); break;
                default: assert(false);
            }
        }
    }

    return std::make_pair(Grid2(std::move(cells)), std::move(directions));
}

void grid_print(const Grid2<Entity> &grid) {
    for (int y = 0; y < grid.rows; y++) {
        for (int x = 0; x < grid.columns; x++) {
            switch (grid.cells[y][x]) {
                case Entity::EmptySpace: std::cout << '.'; break;
                case Entity::Wall: std::cout << '#'; break;
                case Entity::Box: std::cout << 'O'; break;
                case Entity::Robot: std::cout << '@'; break;
                default: assert(false);
            }
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

bool entity_try_move(Grid2<Entity> &grid, const Entity entity, const Vector2 &entity_pos, const Vector2 &direction) {
    const auto potential_pos = entity_pos + direction;
    if (!grid.in_bounds(potential_pos)) {
        return false;
    }

    // Can we move into this space?
    switch (const Entity &other_entity = grid.at(potential_pos)) {
        case Entity::EmptySpace: {
            grid.set(potential_pos, entity);
            grid.set(entity_pos, Entity::EmptySpace);
            return true;
        };
        case Entity::Box: {
            // Try and move the box in front of us before moving into the space
            if (entity_try_move(grid, other_entity, potential_pos, direction)) {
                grid.set(potential_pos, entity);
                grid.set(entity_pos, Entity::EmptySpace);
                return true;
            }
        };
        default: break;
    }

    return false;
}

uint64_t part1(Grid2<Entity> &grid, const std::vector<Vector2> &directions) {
    grid_print(grid);

    Vector2 robot_pos = {0, 0};
    for (int y = 0; y < grid.rows; y++) {
        for (int x = 0; x < grid.columns; x++) {
            if (grid.cells[y][x] == Entity::Robot) {
                robot_pos = Vector2{x, y};
            }
        }
    }

    for (auto direction : directions) {
        if (entity_try_move(grid, Entity::Robot, robot_pos, direction)) {
            robot_pos = robot_pos + direction;
        }
    }

    grid_print(grid);

    uint64_t result = 0;
    for (int y = 0; y < grid.rows; y++) {
        for (int x = 0; x < grid.columns; x++) {
            if (grid.cells[y][x] == Entity::Box) {
                result += y * 100 + x;
            }
        }
    }

    return result;
}

void part2(const Grid2<Entity> &grid, const std::vector<Vector2> &directions) {

}

int main() {
    auto [grid, directions] = read_input("input.txt");
    const size_t part1_result = part1(grid, directions);
    std::cout << "Part 1: " << part1_result << '\n';

    // auto part2_input = read_input("input.txt");
    // part2(part2_input);

    return 0;
}
