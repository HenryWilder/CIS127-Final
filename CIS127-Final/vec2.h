#pragma once
#include "cxmath.h"

struct vec2
{
    vec2() = default;

    constexpr vec2(float x) :
        x(x), y(x) {}

    constexpr vec2(float x, float y) :
        x(x), y(y) {}

    float x, y;

    static constexpr vec2 zero()     noexcept { return vec2(+0.0f); }
    static constexpr vec2 one()      noexcept { return vec2(+1.0f); }
    static constexpr vec2 negative() noexcept { return vec2(-1.0f); }
    static constexpr vec2 right()    noexcept { return vec2(+1.0f, +0.0f); }
    static constexpr vec2 up()       noexcept { return vec2(+0.0f, +1.0f); }
    static constexpr vec2 left()     noexcept { return vec2(-1.0f, +0.0f); }
    static constexpr vec2 down()     noexcept { return vec2(+0.0f, -1.0f); }

    constexpr vec2 operator+() const { return *this; }
    constexpr vec2 operator-() const { return vec2(-x, -y); }

    constexpr vec2 operator+(vec2 other) const { return vec2(x + other.x, y + other.y); }
    constexpr vec2 operator-(vec2 other) const { return vec2(x - other.x, y - other.y); }
    constexpr vec2 operator*(vec2 other) const { return vec2(x * other.x, y * other.y); }
    constexpr vec2 operator/(vec2 other) const { return vec2(x / other.x, y / other.y); }

    constexpr vec2& operator+=(vec2 other) { return *this = (*this + other); }
    constexpr vec2& operator-=(vec2 other) { return *this = (*this - other); }
    constexpr vec2& operator*=(vec2 other) { return *this = (*this * other); }
    constexpr vec2& operator/=(vec2 other) { return *this = (*this / other); }

    constexpr vec2 operator+(float amnt) const { return vec2(x + amnt, y + amnt); }
    constexpr vec2 operator-(float amnt) const { return vec2(x - amnt, y - amnt); }
    constexpr vec2 operator*(float amnt) const { return vec2(x * amnt, y * amnt); }
    constexpr vec2 operator/(float amnt) const { return vec2(x / amnt, y / amnt); }

    constexpr vec2& operator+=(float amnt) { return *this = (*this + amnt); }
    constexpr vec2& operator-=(float amnt) { return *this = (*this - amnt); }
    constexpr vec2& operator*=(float amnt) { return *this = (*this * amnt); }
    constexpr vec2& operator/=(float amnt) { return *this = (*this / amnt); }

    constexpr bool operator==(vec2 other) { return x == other.x && y == other.y; }
    constexpr bool operator!=(vec2 other) { return x != other.x || y != other.y; }

    constexpr float lengthSqr() const { return x * x + y * y; }

    // Magnitude
    constexpr float length() const { return cx::sqrt(lengthSqr()); }

    // Direction
    constexpr vec2 normalized() const { return *this / length(); }

    constexpr vec2 FromAngle(float radians) const { return vec2(cx::cos(radians), cx::sin(radians)); }

    constexpr vec2 rotated(float radians) const
    {
        float cosine = cx::cos(radians);
        float sine = cx::sin(radians);
        return vec2(x * cosine - y * sine, x * sine + y * cosine);
    }
};

static constexpr float dot(vec2 a, vec2 b) { return a.x * b.x + a.y + b.y; }
static constexpr float det(vec2 a, vec2 b) { return a.x * b.y - a.y + b.x; }
static constexpr float distanceSqr(vec2 a, vec2 b) { return (b - a).lengthSqr(); }
static constexpr float distance(vec2 a, vec2 b) { return (b - a).length(); }
static constexpr vec2 direction(vec2 a, vec2 b) { return (b - a).normalized(); }

constexpr bool NearlyEqual(vec2 a, vec2 b, float within = 0.001f) noexcept
{
    return NearlyEqual(a.x, b.x, within) && NearlyEqual(a.y, b.y, within);
}

constexpr vec2 ProjectPointToLine(vec2 lineStart, vec2 lineEnd, vec2 pt)
{
    vec2 dir = direction(lineStart, lineEnd);
    return lineStart + dir * dot(dir, pt - lineStart);
}

static_assert(NearlyEqual(lerp(vec2(0.0f), vec2(1.0f), 0.5f), vec2(0.5f)));
static_assert(NearlyEqual(lerp(vec2(-1.0f, 1.0f), vec2(1.0f, 2.0f), 0.5f), vec2(0.0f, 1.5f)));
