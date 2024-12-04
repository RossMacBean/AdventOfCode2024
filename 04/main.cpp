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

std::vector<std::pair<int, int>> directions {{1,0}, {0,1}, {1, 1}, {-1, 0}, {0, -1}, {-1, -1}, {1, -1}, {-1, 1} };
int part1(const std::vector<std::vector<char>>& board, const std::string &word) {
    int total = 0;

    for (int y = 0; y < board.size(); y++) {
        const auto line = board[y];

        for (int x = 0; x < line.size(); x++) {
            if (board[y][x] != word[0]) {
                continue;
            }

            // Check in all directions from this letter
            for (auto [fst, snd] : directions) {
                // Advance along the direction testing each character as we go
                for (int i = 1; i < word.size(); i++) {
                    const int y2 = y + snd * i;
                    const int x2 = x + fst * i;

                    // Bounds check
                    if (y2 < 0 || y2 >= board.size() || x2 < 0 || x2 >= line.size()) {
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
        }
    }

    return total;
}

int part2(const std::vector<std::vector<char>>& reports) {
    return 0;
}

int main() {
    const auto board = read_input("input.txt");

    const int part1_result = part1(board, "XMAS");
    std::cout << "Part 1: " << part1_result << std::endl;

    // int part2_result = part2(input);
    // std::cout << "Part 2 Simple: " << part2_result << std::endl;


    return 0;
}
