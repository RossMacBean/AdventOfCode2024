#include <algorithm>
#include <cassert>
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

int find_first_unsafe_level_index(const std::vector<int>& levels) {
    const bool is_ascending = levels[0] < levels[1];
    for (int i = 0; i < levels.size() - 1; i++) {
        const int curr = levels[i];
        const int next = levels[i + 1];

        if (!compare_levels(curr, next, is_ascending)) {
            return i;
        }
    }

    return -1;
}

bool is_report_safe_with_ignored_level(const std::vector<int>& levels, const int ignored_index) {
    int start_index = 0;
    int end_index = static_cast<int>(levels.size()) - 1;

    if (ignored_index == 0) {
        start_index = 1;
    }

    if (ignored_index == levels.size() - 1) {
        end_index = end_index - 1;
    }

    assert(start_index < end_index);
    assert(start_index < levels.size() - 1);

    const bool is_ascending = levels[start_index] < levels[start_index + 1];
    for (int i = start_index; i <= end_index; i++) {
        // If the current element is the ignored one, then skip this iteration
        if (i == ignored_index) {
            continue;
        }

        int next_index = i + 1;
        // If the next pointer is pointing to the ignored element, then skip over to the following one
        if (next_index == ignored_index) {
            next_index++;
        }

        if (next_index > end_index) {
            break;
        }

        if (!compare_levels(levels[i], levels[next_index], is_ascending)) {
            return false;
        }
    }

    return true;
}

int part1(std::vector<std::vector<int>>& reports) {
    int safe_reports_count = 0;

    for (const auto& report : reports) {
        if (report.size() < 2) {
            safe_reports_count++;
            continue;
        }

        if (find_first_unsafe_level_index(report) == -1) {
            safe_reports_count++;
        }
    }

    return safe_reports_count;
}

int part2_simple(const std::vector<std::vector<int>>& reports) {
    int safe_reports_count = 0;

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

        // Brute force, try every combination of the report
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

int part2_optimised(const std::vector<std::vector<int>>& reports) {
    int safe_reports_count = 0;

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

        // Only need to check either size of the level pair that caused the failure
        // Let 'a' be the index at which the first unsafe level was found and let 'b' be the level it was being compared to, which is the following element
        // We need to check in the range [a-1, b+1] which is the same as [a-1, a+2]
        const int start_index = std:: max(first_unsafe_level_index - 1, 0);
        const int end_index = std::min(first_unsafe_level_index + 2, static_cast<int>(reports.size() - 1));

        for (int j = start_index; j < end_index; j++) {
            if (is_report_safe_with_ignored_level(report, j)) {
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

    int part2_result = part2_optimised(input);
    std::cout << "Part 2 Simple: " << part2_result << std::endl;

    part2_result = part2_optimised(input);
    std::cout << "Part 2 Optimised: " << part2_result << std::endl;

    return 0;
}
