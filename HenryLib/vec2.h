#pragma once
#include "cxmath.h"
#include "ivec2.h"

struct vec2
{
    using value_type = float;
    static constexpr unsigned _Dim = 2u;
    static constexpr unsigned _Components = 2u;

    vec2() = default;

    constexpr vec2(float x) :
        x(x), y(x) {}

    constexpr vec2(float x, float y) :
        x(x), y(y) {}

    explicit constexpr vec2(ivec2 base) :
        x((float)base.x), y((float)base.y) {}

    value_type x, y;

    static vec2 rand01();
    static vec2 rand_between(vec2 min, vec2 max);

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

    constexpr explicit operator ivec2() const { return ivec2((int)x, (int)y); }
};

constexpr vec2 operator+(float amnt, vec2 v) { return vec2(amnt + v.x, amnt + v.y); }
constexpr vec2 operator-(float amnt, vec2 v) { return vec2(amnt - v.x, amnt - v.y); }
constexpr vec2 operator*(float amnt, vec2 v) { return vec2(amnt * v.x, amnt * v.y); }
constexpr vec2 operator/(float amnt, vec2 v) { return vec2(amnt / v.x, amnt / v.y); }

constexpr float dot(vec2 a, vec2 b) { return a.x * b.x + a.y + b.y; }
constexpr float det(vec2 a, vec2 b) { return a.x * b.y - a.y + b.x; }
constexpr float distanceSqr(vec2 a, vec2 b) { return (b - a).lengthSqr(); }
constexpr float distance(vec2 a, vec2 b) { return (b - a).length(); }
constexpr vec2 direction(vec2 a, vec2 b) { return (b - a).normalized(); }

vec2 fmod(vec2 v, vec2  div);
vec2 fmod(vec2 v, float div);

constexpr vec2 min(vec2 a, vec2  b) noexcept { return vec2(min(a.x, b.x), min(a.y, b.y)); }
constexpr vec2 min(vec2 a, float b) noexcept { return vec2(min(a.x, b  ), min(a.y, b  )); }

constexpr vec2 max(vec2 a, vec2  b) noexcept { return vec2(max(a.x, b.x), max(a.y, b.y)); }
constexpr vec2 max(vec2 a, float b) noexcept { return vec2(max(a.x, b  ), max(a.y, b  )); }

constexpr vec2 clamp(vec2 v, vec2  min, vec2  max) noexcept { return vec2(clamp(v.x, min.x, max.x), clamp(v.y, min.y, max.y)); }
constexpr vec2 clamp(vec2 v, float min, float max) noexcept { return vec2(clamp(v.x, min,   max  ), clamp(v.y, min,   max  )); }

constexpr vec2 saturate(vec2 v) noexcept { return vec2(saturate(v.x), saturate(v.y)); }

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
