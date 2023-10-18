#pragma once
#include "cxmath.h"
#include "vec2.h"

struct rect
{
    using value_type = float;
    static constexpr int _Size = 4;

    rect() = default;

    constexpr rect(float x, float y, float size) :
        xmin(x), ymin(y), xmax(x + size), ymax(y + size) {}

    constexpr rect(vec2 position, float size) :
        xmin(position.x), ymin(position.y), xmax(position.x + size), ymax(position.y + size) {}

    constexpr rect(float xmin, float ymin, float xmax, float ymax) :
        xmin(xmin), ymin(ymin), xmax(xmax), ymax(ymax) {}

    value_type xmin, ymin, xmax, ymax;

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

    constexpr vec2    TopLeft () const noexcept { return vec2(xmin, ymin); }
    constexpr vec2    TopRight() const noexcept { return vec2(xmax, ymin); }
    constexpr vec2 BottomLeft () const noexcept { return vec2(xmin, ymax); }
    constexpr vec2 BottomRight() const noexcept { return vec2(xmax, ymax); }

    constexpr vec2 Center() const noexcept { return average(TopLeft(), BottomRight()); }

    constexpr float width () const noexcept { return xmax - xmin; }
    constexpr float height() const noexcept { return ymax - ymin; }
    constexpr vec2 size()    const noexcept { return vec2(width(), height()); }
    constexpr vec2 extents() const noexcept { return size() * 0.5f; }
};
