#pragma once
#include "cxmath.h"
#include "ivec2.h"

struct irect
{
    using value_type = int;
    static constexpr int _Size = 4;

    irect() = default;

    constexpr irect(int xmin, int ymin, int xmax, int ymax) :
        xmin(xmin), ymin(ymin), xmax(xmax), ymax(ymax) {}

    value_type xmin, ymin, xmax, ymax;

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

    constexpr ivec2    TopLeft () const noexcept { return ivec2(xmin, ymin); }
    constexpr ivec2    TopRight() const noexcept { return ivec2(xmax, ymin); }
    constexpr ivec2 BottomLeft () const noexcept { return ivec2(xmin, ymax); }
    constexpr ivec2 BottomRight() const noexcept { return ivec2(xmax, ymax); }

    constexpr int width () const noexcept { return xmax - xmin; }
    constexpr int height() const noexcept { return ymax - ymin; }
    constexpr ivec2 size()    const noexcept { return ivec2(width(), height()); }
    constexpr ivec2 extents() const noexcept { return size() / 2; } // Doesn't work correctly for odd-number sizes.
};
