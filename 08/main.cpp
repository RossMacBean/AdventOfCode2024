#include <cassert>
#include <format>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>

struct Point {
    int32_t row;
    int32_t col;

    bool operator==(const Point &other) const {
        return row == other.row && col == other.col;
    }
};

std::vector<std::vector<char>> read_input(const std::string &filename) {
    std::ifstream file{filename};

    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
    }

    std::vector<std::vector<char>> result;
    std::string line;
    while (std::getline(file, line)) {
        std::vector row(line.begin(), line.end());
        result.emplace_back(row);
    }

    return result;
}

void print_board(const std::vector<std::vector<char>> &board) {
    for (const auto &row : board) {
        for (const char c : row) {
            std::cout << c;
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
}

std::unordered_map<char, std::vector<Point>> build_node_map(const std::vector<std::vector<char>> &board)
{
    std::unordered_map<char, std::vector<Point>> nodes_map;
    for (int j = 0; j < board.size(); j++) {
        for (int i = 0; i < board[j].size(); i++) {
            if (board[j][i] != '.') {
                nodes_map[board[j][i]].push_back({ .row = j, .col = i });
            }
        }
    }

    return nodes_map;
}

std::vector<std::vector<char>> init_empty_node_board(const size_t rows, const size_t cols) {
    std::vector<std::vector<char>> board(rows);
    for (int j = 0; j < rows; j++) {
        board[j] = std::vector(cols, '.');
    }

    return board;
}

bool bounds_check(const std::vector<std::vector<char>> &board, const Point &p) {
    return p.row >= 0 && p.row < board.size()&& p.col >= 0 && p.col < board[p.row].size();
}

std::vector<Point> calculate_anti_node_points(const std::vector<std::vector<char>> &board, const Point a, const Point b) {
    std::vector<Point> anti_nodes_points;

    const int32_t row_diff = a.row - b.row;
    const int32_t col_diff = a.col - b.col;

    // First anti-node
    if (const Point p(a.row + row_diff, a.col + col_diff); bounds_check(board, p)) {
        anti_nodes_points.emplace_back(p);
    }

    // Second anti-node
    if (const Point p(b.row + row_diff * -1, b.col + col_diff * -1); bounds_check(board, p)) {
        anti_nodes_points.emplace_back(p);
    }

    return anti_nodes_points;
}

int64_t part1(const std::vector<std::vector<char>> &board) {
    auto nodes_map = build_node_map(board);
    auto anti_node_board = init_empty_node_board(board.size(), board[0].size());

    for (const auto& [node_type, node_list] : nodes_map) {
        for (int i = 0; i < node_list.size(); i++) {
            for (int j = i + 1; j < node_list.size(); j++) {
                auto anti_node_points= calculate_anti_node_points(board, node_list[i], node_list[j]);

                for (auto [row, col] : anti_node_points) {
                    anti_node_board[row][col] = '#';
                }
            }
        }
    }

    // Count number of anti-nodes
    int64_t total = 0;
    for (const auto& row : anti_node_board) {
        for (const auto cell : row) {
            if (cell == '#') total++;
        }
    }

    return total;
}

std::vector<Point> calculate_anti_node_points_with_harmonics(const std::vector<std::vector<char>> &board, const Point a, const Point b) {
    std::vector anti_nodes_points{a, b};

    const int32_t base_row_diff = a.row - b.row;
    const int32_t base_col_diff = a.col - b.col;

    // First anti-node
    for (int i = 1; ; i++) {
        const int32_t row_diff = base_row_diff * i;
        const int32_t col_diff = base_col_diff * i;

        const Point p(a.row + row_diff, a.col + col_diff);
        if (!bounds_check(board, p)) {
            break;
        }

        anti_nodes_points.emplace_back(p);
    }

    // Second anti-node
    for (int i = 1; ; i++) {
        const int32_t row_diff = base_row_diff * i;
        const int32_t col_diff = base_col_diff * i;

        const Point p(b.row + row_diff * -1, b.col + col_diff * -1);
        if (!bounds_check(board, p)) {
            break;
        }

        anti_nodes_points.emplace_back(p);
    }

    return anti_nodes_points;
}

int64_t part2(const std::vector<std::vector<char>> &board) {
    auto nodes_map = build_node_map(board);
    auto anti_node_board = init_empty_node_board(board.size(), board[0].size());

    for (const auto& [node_type, node_list] : nodes_map) {
        for (int i = 0; i < node_list.size(); i++) {
            for (int j = i + 1; j < node_list.size(); j++) {
                for (auto [row, col] : calculate_anti_node_points_with_harmonics(board, node_list[i], node_list[j])) {
                    anti_node_board[row][col] = '#';
                }
            }
        }
    }

    // Count number of anti-nodes
    int64_t total = 0;
    for (const auto& row : anti_node_board) {
        for (const auto cell : row) {
            if (cell == '#') total++;
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
