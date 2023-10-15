#pragma once
#include "cxmath.h"
#include "vec2.h"

struct line
{
    using value_type = vec2;
    static constexpr int _Size = 2;

    vec2 start, end;
};

struct triangle
{
    using value_type = vec2;
    static constexpr int _Size = 3;

    triangle() = default;

    constexpr triangle(vec2 p1, vec2 p2, vec2 p3) :
        points{ p1, p2, p3 } {}

    vec2 points[3];
};

struct quad
{
    using value_type = vec2;
    static constexpr int _Size = 4;

    quad() = default;

    constexpr quad(vec2 p1, vec2 p2, vec2 p3, vec2 p4) :
        points{ p1, p2, p3, p4 } {}

    vec2 points[4]; // Clockwise order with first point being UV(0,0) (top left of a texture)

    constexpr vec2 CoordToPos(vec2 uv) const
    {
        const auto&[topLeft, topRight, bottomRight, bottomLeft] = points;
        vec2 top = lerp(topLeft, topRight, uv.x);
        vec2 bottom = lerp(bottomLeft, bottomRight, uv.x);
        return lerp(top, bottom, uv.y);
    }
};

struct circle
{
    circle() = default;

    constexpr circle(vec2 position, float radius) :
        position(position), radius(radius) {}

    vec2 position;
    float radius;
};

struct oval
{
    oval() = default;

    constexpr oval(vec2 position, float r1, float r2) :
        position(position), r1(r1), r2(r2) {}

    vec2 position;
    float r1, r2;
};
