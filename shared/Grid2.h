#ifndef GRID2_H
#define GRID2_H
#include <cstdint>
#include <vector>

template<typename T>
class Grid2 {
public:
    explicit Grid2(std::vector<std::vector<T>> cells)
        : rows(cells.size()), columns(cells[0].size()), cells(cells) {}

    [[nodiscard]] bool in_bounds(const Vector2 point) const {
        return point.y >= 0 && point.y < rows && point.x >= 0 && point.x < columns;
    }

    const T& at(const Vector2 point) const {
        assert(in_bounds(point));
        return cells[point.y][point.x];
    }

    void set(Vector2 vector2, T cell) {
        assert(in_bounds(vector2));
        cells[vector2.y][vector2.x] = cell;
    }

    int32_t rows, columns;
    std::vector<std::vector<T>> cells;
};

#endif //GRID2_H
