#include "Vector2.h"

bool Vector2::operator==(const Vector2& other) const {
    return x == other.x && y == other.y;
}