#include <algorithm>
#include <format>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

struct rule {
    int32_t before;
    int32_t after;
};

struct input {
    // There is probably a much better way of structuring the rules than just a flat list of pairs. Every rule test
    // requires iterating the entire vector.
    const std::vector<rule> rules;

    std::vector<std::vector<int32_t>> updates;
};

input read_input(const std::string &filename) {
    std::ifstream file{filename};

    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
    }

    std::vector<rule> rules;

    std::string line;
    std::getline(file, line);
    while (!line.empty()) {
        const auto pipe_index = line.find('|');

        const int32_t before = std::stoi(line.substr(0, pipe_index));
        const int32_t after = std::stoi(line.substr(pipe_index + 1));

        rules.push_back({ before, after });
        std::getline(file, line);
    }

    std::vector<std::vector<int32_t>> updates;
    while (std::getline(file, line)) {
        std::vector<int32_t> update;

        size_t pos = 0;
        while ((pos = line.find(',')) != std::string::npos) {
            update.push_back(std::stoi(line.substr(0, pos)));
            line.erase(0, pos + 1);
        }
        update.push_back(std::stoi(line));
        updates.push_back(update);
    }

    return { std::move(rules), std::move(updates) };
}

bool test_rule(const int32_t a, const int32_t b, const std::vector<rule> &rules) {
    return std::ranges::find_if(rules, [&](const auto &rule) {
        return rule.after == a && rule.before == b;
    }) == rules.end();
}

std::optional<std::pair<int32_t, int32_t>> find_first_bad_update_pair(const std::vector<int32_t> &updates, const std::vector<rule> &rules) {
    for (int32_t updI = 0; updI < updates.size(); updI++) {
        for (int32_t otherI = updI + 1; otherI < updates.size(); otherI++) {
            if (!test_rule(updates[updI], updates[otherI], rules)) {
                return { { updI, otherI } };
            }
        }
    }

    return {};
}

std::vector<int32_t> correct_update(const std::vector<int32_t> &update, const std::vector<rule> &rules) {
    std::vector result(update);

    std::ranges::sort(result, [&](const auto &a, const auto &b) {
        return test_rule(a, b, rules);
    });

    // // oh look, a bubble sort!
    // auto bad_pair = find_first_bad_update_pair(result, rules);
    // while (bad_pair) {
    //     const auto t = result[bad_pair->first];
    //     result[bad_pair->first] = result[bad_pair->second];
    //     result[bad_pair->second] = t;
    //
    //     bad_pair = find_first_bad_update_pair(result, rules);
    // }

    return result;
}

int part1(const input& input) {
    int32_t total = 0;

    for (auto &update : input.updates) {
        // If there are no bad pairs then this is a valid update
        if (!find_first_bad_update_pair(update, input.rules).has_value()) {
            total += update[(update.size() - 1) / 2];
        }
    }

    return total;
}

int part2(const input& input) {
    int32_t total = 0;

    for (auto &update : input.updates) {
        if (find_first_bad_update_pair(update, input.rules)) {
            // Re-order the update so it's in the right order
            auto corrected_update = correct_update(update, input.rules);

            // Then take the midpoint and add it to the total
            total += corrected_update[(update.size() - 1) / 2];
        }
    }

    return total;
}

int main() {
    const auto input = read_input("input.txt");

    const int part1_result = part1(input);
    std::cout << "Part 1: " << part1_result << std::endl;

    const auto part2_input = read_input("input.txt");
    const int part2_result = part2(part2_input);
    std::cout << "Part 2: " << part2_result << std::endl;

    return 0;
}
