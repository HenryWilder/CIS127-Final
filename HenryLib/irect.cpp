#include "irect.h"
#include "constants.h"

irect_scanline_iterator::irect_scanline_iterator(const irect& parent, ivec2 value) :
    parent(parent), value(value) {}

ivec2 irect_scanline_iterator::operator*() const
{
    return value;
}

void irect_scanline_iterator::operator++()
{
    ++value.x;
    if (value.x > parent.xmax)
    {
        value.x = parent.xmin;
        ++value.y;
    }
}

bool irect_scanline_iterator::operator!=(const irect_scanline_iterator& other) const
{
    return (&parent != &other.parent) || (value != other.value);
}

irect_scanline_iterator irect::begin()
{
    return irect_scanline_iterator(*this, TopLeft());
}

irect_scanline_iterator irect::end() const
{
    return irect_scanline_iterator(*this, BottomLeft() + down<ivec2>);
}
