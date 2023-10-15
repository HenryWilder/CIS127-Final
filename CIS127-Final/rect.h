#pragma once
#include "cxmath.h"
#include "vec2.h"

struct rect
{
    rect() = default;

    constexpr rect(float x, float y, float size) :
        xmin(x), ymin(y), xmax(x + size), ymax(y + size) {}

    constexpr rect(vec2 position, float size) :
        xmin(position.x), ymin(position.y), xmax(position.x + size), ymax(position.y + size) {}

    constexpr rect(float xmin, float ymin, float xmax, float ymax) :
        xmin(xmin), ymin(ymin), xmax(xmax), ymax(ymax) {}

    float xmin, ymin, xmax, ymax;

    static constexpr rect FromMinAndMax(vec2 min, vec2 max) noexcept
    {
        return rect(min.x, min.y, max.x, max.y);
    }

    static constexpr rect FromPositionAndSize(vec2 pos, vec2 size) noexcept
    {
        return rect(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
    }

    static constexpr rect FromPositionAndSize(vec2 pos, float size) noexcept
    {
        return rect(pos.x, pos.y, pos.x + size, pos.y + size);
    }
};