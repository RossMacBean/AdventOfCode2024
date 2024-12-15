#include "Part2.h"

#include <cstdint>
#include <iostream>
#include <set>
#include <string>
#include <vector>

namespace Part2
{

FileSystem read_input(const std::string &input) {
    std::vector<File> files;
    std::vector<Gap> gaps;

    bool is_file = true;
    int64_t file_id = 0;
    size_t current_pos = 0;
    for (const auto &c : input) {
        if (c < '0' || c > '9') {
            std::cerr << "Invalid character found in input stream. All values should be 0 - 9" << std::endl;
            exit(1);
        }

        uint64_t block_length = c - '0';

        if (is_file) {
            files.emplace_back(current_pos, block_length, file_id);
            file_id++;
        } else {
            gaps.emplace_back(current_pos, block_length);
        }

        current_pos += block_length;
        is_file = !is_file;
    }

    return { std::move(files), std::move(gaps) };
}

void FileSystem::reorder() {
    for (size_t i = Files.size() - 1; i > 0; i--) {
        auto &file = Files[i];

        // Find first free gap
        Gap* swap_gap = nullptr;
        for (auto &Gap : Gaps) {
            if (Gap.len >= file.len && Gap.pos < file.pos) {
                swap_gap = &Gap;
                break;
            }
        }

        // No gap, no swap
        if (swap_gap == nullptr) {
            continue;
        }

        // Move the file into the gap
        file.pos = swap_gap->pos;

        // Update the gap with the left over space
        const size_t left_over_space = swap_gap->len - file.len;
        swap_gap->len = left_over_space;
        swap_gap->pos = file.pos + file.len;
    }
}

uint64_t FileSystem::calculate_checksum() {
    uint64_t checksum = 0;

    for (const auto &file : Files) {
        for (size_t i = file.pos; i < file.pos + file.len; i++) {
            checksum += i * file.file_id;
        }
    }

    return checksum;
}

}
