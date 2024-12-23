#ifndef DIRECTION_H
#define DIRECTION_H

#include "Vector2.h"

namespace Direction {
    static constexpr auto Up = Vector2(0, -1);
    static constexpr auto Right = Vector2(1, 0);
    static constexpr auto Down = Vector2(0, 1);
    static constexpr auto Left = Vector2(-1, 0);
    static constexpr std::array Cardinals = { Up, Down, Left, Right };
}

#endif //DIRECTION_H
