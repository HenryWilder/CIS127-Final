#pragma once

struct IVec2
{
    IVec2() = default;

    constexpr IVec2(int x, int y) :
        x(x), y(y) {}

    constexpr IVec2 operator+(IVec2 offset) const
    {
        return { x + offset.x, y + offset.y };
    }

    constexpr bool operator==(IVec2 offset) const
    {
        return x == offset.x && y == offset.y;
    }

    int x, y;
};
