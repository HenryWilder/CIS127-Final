#pragma once

constexpr float Clamp(float x, float min, float max)
{
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

constexpr float Lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

struct vec2
{
    constexpr vec2() : x(), y() {}
    constexpr vec2(float x) : x(x), y(x) {}
    constexpr vec2(float x, float y) : x(x), y(y) {}
    float x, y;
};

constexpr vec2 operator+(vec2 a, vec2 b) { return { a.x + b.x, a.y + b.y }; }
constexpr vec2 operator-(vec2 a, vec2 b) { return { a.x - b.x, a.y - b.y }; }
constexpr vec2 operator*(vec2 a, vec2 b) { return { a.x * b.x, a.y * b.y }; }
constexpr vec2 operator/(vec2 a, vec2 b) { return { a.x / b.x, a.y / b.y }; }
constexpr vec2 operator*(vec2 a, float scale) { return { a.x * scale, a.y * scale }; }
constexpr vec2 operator/(vec2 a, float scale) { return { a.x / scale, a.y / scale }; }

constexpr float Dot(vec2 a, vec2 b)
{
    return a.x * b.x + a.y * b.y;
}

constexpr float Det(vec2 a, vec2 b)
{
    return a.x * b.y - a.y * b.x;
}

constexpr float LengthSqr(vec2 a)
{
    return Dot(a, a);
}

float Length(vec2 a);

constexpr float DistanceSqr(vec2 a, vec2 b)
{
    return LengthSqr(a.x - b.x);
}

float Distance(vec2 a, vec2 b);

vec2 Normalize(vec2 v);

constexpr vec2 Lerp(vec2 a, vec2 b, float t)
{
    return a + (b - a) * t;
}

vec2 ProjectPointToLine(vec2 lineStart, vec2 lineEnd, vec2 pt);

struct quad
{
    // Ordered clockwise
    vec2 topLeft, topRight, bottomRight, bottomLeft;

    constexpr vec2 CoordToPos(vec2 pt) const
    {
        return Lerp(Lerp(topLeft, topRight, pt.x), Lerp(bottomLeft, bottomRight, pt.x), pt.y);
    }
};
