#ifndef PART2_H
#define PART2_H
#include <cstdint>
#include <string>
#include <vector>

namespace Part2
{

struct File {
    size_t pos{};
    size_t len{};
    uint64_t file_id{};
};

struct Gap {
    size_t pos{};
    size_t len{};
};

struct FileSystem {
    std::vector<File> Files;
    std::vector<Gap> Gaps;

    void reorder();
    uint64_t calculate_checksum();
};

FileSystem read_input(const std::string &input);

}

#endif //PART2_H
