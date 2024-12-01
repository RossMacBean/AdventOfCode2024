#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

struct input_lists {
    std::vector<int> left;
    std::vector<int> right;
};

input_lists read_input() {
    std::ifstream input{"input.txt"};

    if (!input) {
        std::cerr << "Error opening file" << std::endl;
    }

    std::vector<int> left;
    std::vector<int> right;

    std::string line;
    while (!input.eof()) {
        std::getline(input, line);

        // find first white space
        const auto whiteSpacePos = line.find(' ');

        // find first character after white space
        const auto rightPos = line.find_first_not_of(' ', whiteSpacePos);

        left.push_back(std::stoi(line.substr(0, whiteSpacePos)));
        right.push_back(std::stoi(line.substr(rightPos)));
    }

    assert(left.size() == right.size());
    return { left, right };
}

int part1() {
    auto input = read_input();

    std::ranges::sort(input.left);
    std::ranges::sort(input.right);

    int total = 0;
    for (auto i = 0; i < input.left.size(); i++) {
        auto result = std::abs(input.left[i] - input.right[i]);
        total += result;
    }

    return total;
}

int part2() {
    auto input = read_input();

    std::map<int, int> right_freq;
    for (auto i : input.right) {
        right_freq[i]++;
    }

    int total = 0;
    for (auto i : input.left) {
        int freq = 0;
        if (auto iter = right_freq.find(i); iter != right_freq.end()) {
            freq =  iter->second;
        };

        total += i * freq;
    }

    return total;
}

int main() {
    const int part1_result = part1();
    std::cout << "Part 1: " << part1_result << std::endl;

    const int part2_result = part2();
    std::cout << "Part 2: " << part2_result << std::endl;

    return 0;
}
