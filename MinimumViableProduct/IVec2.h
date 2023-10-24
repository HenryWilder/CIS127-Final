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

#include <xhash>

// This is how we allow position-based unordered_map
namespace std
{
    template<>
    struct hash<IVec2>
    {
        size_t operator()(const IVec2 v) const noexcept
        {
            size_t hashResult = ((size_t)v.y << (8ull * sizeof(int))) | (size_t)v.x;
            return hashResult;
        }
    };
}
