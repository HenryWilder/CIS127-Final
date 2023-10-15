#pragma once
#include "cxmath.h"
#include "ivec2.h"
#include "vec2.h"

struct irect
{
    irect() = default;

    constexpr irect(int xmin, int ymin, int xmax, int ymax) :
        xmin(xmin), ymin(ymin), xmax(xmax), ymax(ymax) {}

    int xmin, ymin, xmax, ymax;

    static constexpr irect FromMinAndMax(ivec2 min, ivec2 max) noexcept
    {
        return irect(min.x, min.y, max.x, max.y);
    }

    static constexpr irect FromPositionAndSize(ivec2 pos, ivec2 size) noexcept
    {
        return irect(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
    }

    static constexpr irect FromPositionAndSize(ivec2 pos, int size) noexcept
    {
        return irect(pos.x, pos.y, pos.x + size, pos.y + size);
    }
};
