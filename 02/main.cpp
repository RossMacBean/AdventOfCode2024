#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::vector<int>> read_input() {
    std::ifstream file{"input.txt"};

    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
    }

    std::vector<std::vector<int>> result;

    std::string line;
    while (std::getline(file, line)) {
        std::vector<int> row;
        size_t pos = 0;

        while ((pos = line.find(' ')) != std::string::npos) {
            row.push_back(std::stoi(line.substr(0, pos)));
            line.erase(0, pos + 1);
        }
        row.push_back(std::stoi(line)); // Consume final value

        result.push_back(row);
    }

    return result;
}

std::string report_to_string(const std::vector<int>& report) {
    std::string result;
    for (const auto level : report) {
        result += std::to_string(level) + ' ';
    }

    return result;
}

// Returns true if the two levels are safe, false if not
bool compare_levels(const int a, const int b, const bool is_ascending) {
    const int diff = a - b;
    if (is_ascending && diff > 0) {
        return false;
    }

    if (!is_ascending && diff < 0) {
        return false;
    }

    if (int abs_diff = std::abs(diff); abs_diff < 1 || abs_diff > 3) {
        return false;
    }

    return true;
}

int  find_first_unsafe_level_index(const std::vector<int>& levels) {
    const bool is_ascending = levels[0] < levels[1];
    for (int i = 1; i < levels.size(); i++) {
        const int prev = levels[i - 1];
        const int curr = levels[i];

        if (!compare_levels(prev, curr, is_ascending)) {
            return i - 1;
        }
    }

    return -1;
}

int part1(std::vector<std::vector<int>>& reports) {
    int safe_reports_count = 0;

    for (const auto& report : reports) {
        if (report.size() < 2) {
            continue;
        }

        if (find_first_unsafe_level_index(report) == -1) {
            safe_reports_count++;
        }
    }

    return safe_reports_count;
}

int part2(const std::vector<std::vector<int>>& reports) {
    int safe_reports_count = 0;
    std::vector<std::vector<int>> unsafe_reports(reports.size());

    for (const auto& report : reports) {
        if (report.size() < 2) {
            safe_reports_count++;
            continue;
        }

        const auto first_unsafe_level_index = find_first_unsafe_level_index(report);
        if (first_unsafe_level_index == -1) {
            safe_reports_count++;
            continue;
        }

        // Disgusting brute force, "clever" solutions all ran in to an edge cases where the first element is the one that can be removed
        for (int j = 0; j < report.size(); j++) {
            auto new_report = report;
            new_report.erase(new_report.begin() + j);

            if (find_first_unsafe_level_index(new_report) == -1) {
                safe_reports_count++;
                break;
            }
        }
    }

    return safe_reports_count;
}

int main() {
    auto input = read_input();
    const int part1_result = part1(input);
    std::cout << "Part 1: " << part1_result << std::endl;

    const int part2_result = part2(input);
    std::cout << "Part 2: " << part2_result << std::endl;

    return 0;
}
