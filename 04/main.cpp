#include <format>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::vector<char>> read_input(const std::string &filename) {
    std::ifstream file{filename};

    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
    }

    std::vector<std::vector<char>> result;

    std::string line;
    while (std::getline(file, line)) {
        std::vector<char> line_chars;
        line_chars.insert(line_chars.end(), line.begin(), line.end());

        result.push_back(line_chars);
    }

    return result;
}

int check_tile(
    const int y,
    const int x,
    const std::vector<std::vector<char>>& board,
    const std::string &word,
    const std::vector<std::pair<int,int>>& directions)
{
    if (y < 0 || y >= board.size() || x < 0 || x >= board[y].size()) {
        return 0;
    }

    if (board[y][x] != word[0]) {
        return 0;
    }

    int total = 0;

    // Check in all directions from this tile
    for (auto [fst, snd] : directions) {
        // Advance along the direction testing each character as we go
        for (int i = 1; i < word.size(); i++) {
            const int y2 = y + fst * i;
            const int x2 = x + snd * i;

            // Bounds check
            if (y2 < 0 || y2 >= board.size() || x2 < 0 || x2 >= board[y2].size()) {
                break;
            }

            // Check if the letter matches
            if (board[y2][x2] != word[i]) {
                break;
            }

            if (i == word.size() - 1) {
                // Found a complete word
                total++;
            }
        }
    }

    return total;
}

std::vector<std::pair<int, int>> directions_part1 {{1,0}, {0,1}, {1, 1}, {-1, 0}, {0, -1}, {-1, -1}, {1, -1}, {-1, 1} };
int part1(const std::vector<std::vector<char>>& board) {
    int total = 0;

    for (int y = 0; y < board.size(); y++) {
        for (int x = 0; x < board[y].size(); x++) {
            total += check_tile(y, x, board, "XMAS", directions_part1);
        }
    }

    return total;
}

int part2(const std::vector<std::vector<char>>& board) {
    int total = 0;

    for (int y = 0; y < board.size(); y++) {
        const auto& line = board[y];

        for (int x = 0; x < line.size(); x++) {
            int mas_count = 0;

            // Find an 'A' tile, and then check for "MAS" in the 4 corners
            if (board[y][x] != 'A') {
                continue;
            }

            // Top left
            if (check_tile(y -1, x -1, board, "MAS", { {1, 1}})) {
                mas_count++;
            }

            // Top right
            if (check_tile(y -1 , x + 1, board, "MAS", { {1, -1}})) {
                mas_count++;
            }

            // Bottom left
            if (check_tile(y + 1, x - 1, board, "MAS", { {-1, 1}})) {
                mas_count++;
            }

            // Bottom right
            if (check_tile(y + 1, x + 1, board, "MAS", { {-1, -1}})) {
                mas_count++;
            }

            // If there's 2 "MAS"es, then there's an X-MAS
            if (mas_count == 2) {
                total++;
            }
        }
    }

    return total;
}

int main() {
    const auto board = read_input("input.txt");

    const int part1_result = part1(board);
    std::cout << "Part 1: " << part1_result << std::endl;

    const auto part2_board = read_input("input.txt");
    const int part2_result = part2(part2_board);
    std::cout << "Part 2: " << part2_result << std::endl;


    return 0;
}
