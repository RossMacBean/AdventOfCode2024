#ifndef VECTOR2_H
#define VECTOR2_H
#include <cstdint>
#include <functional>

struct Vector2 {
    int32_t x, y;

    bool operator==(const Vector2 &other) const;
};

template <>
struct std::hash<Vector2> {
    size_t operator()(const Vector2& p) const noexcept {
        // Large primes
        return p.x * 73856093 ^ p.y * 19349663;
    }
};

#endif //VECTOR2_H
