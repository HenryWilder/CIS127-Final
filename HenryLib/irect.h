#pragma once
#include "cxmath.h"
#include "ivec2.h"

struct irect;

class irect_scanline_iterator
{
public:
    irect_scanline_iterator(const irect& parent, ivec2 value);

    ivec2 operator*() const;
    void operator++();
    bool operator!=(const irect_scanline_iterator& other) const;

private:
    const irect& parent;
    ivec2 value;
};

struct irect
{
    using value_type = int;
    static constexpr unsigned _Dim = 2u;
    static constexpr unsigned _Components = 4u;

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

    constexpr ivec2    TopLeft () const noexcept { return ivec2(xmin, ymin); }
    constexpr ivec2    TopRight() const noexcept { return ivec2(xmax, ymin); }
    constexpr ivec2 BottomLeft () const noexcept { return ivec2(xmin, ymax); }
    constexpr ivec2 BottomRight() const noexcept { return ivec2(xmax, ymax); }

    constexpr int width () const noexcept { return xmax - xmin; }
    constexpr int height() const noexcept { return ymax - ymin; }
    constexpr ivec2 size()    const noexcept { return ivec2(width(), height()); }
    constexpr ivec2 extents() const noexcept { return size() / 2; } // Doesn't work correctly for odd-number sizes.

    irect_scanline_iterator begin();
    irect_scanline_iterator end() const;
};

constexpr irect clamp(irect rec, ivec2 vmin, ivec2 vmax)
{
    return irect(
        max(rec.xmin, vmin.x),
        max(rec.ymin, vmin.x),
        min(rec.xmax, vmax.x),
        min(rec.ymax, vmax.x));
}

constexpr irect clamp(irect rec, irect minmax)
{
    return irect(
        max(rec.xmin, minmax.xmin),
        max(rec.ymin, minmax.ymin),
        min(rec.xmax, minmax.xmax),
        min(rec.ymax, minmax.ymax));
}

constexpr irect clamp(irect rec, int xmin, int ymin, int xmax, int ymax)
{
    return irect(
        max(rec.xmin, xmin),
        max(rec.ymin, ymin),
        min(rec.xmax, xmax),
        min(rec.ymax, ymax));
}
