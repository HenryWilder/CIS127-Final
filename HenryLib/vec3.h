#pragma once
#include "cxmath.h"
#include "vec2.h"

struct vec3
{
    using value_type = float;
    static constexpr int _Size = 3;

    vec3() = default;

    constexpr vec3(float x) :
        x(x), y(x), z(x) {}

    // Two-element initialization of a vec3 isn't very intuitive, so I'm disallowing it to prevent misbehavior.
    vec3(float x, float y) = delete;

    constexpr vec3(vec2 xy, float z) :
        x(xy.x), y(xy.y), z(z) {}

    constexpr vec3(float x, float y, float z) :
        x(x), y(y), z(z) {}

    value_type x, y, z;

    static consteval vec3 zero()     noexcept { return { +0.0f, +0.0f, +0.0f }; }
    static consteval vec3 one()      noexcept { return { +1.0f, +1.0f, +1.0f }; }
    static consteval vec3 negative() noexcept { return { -1.0f, -1.0f, -1.0f }; }
    static consteval vec3 right()    noexcept { return { +1.0f, +0.0f, +0.0f }; }
    static consteval vec3 up()       noexcept { return { +0.0f, +1.0f, +0.0f }; }
    static consteval vec3 forward()  noexcept { return { +0.0f, +0.0f, +1.0f }; }
    static consteval vec3 left()     noexcept { return { -1.0f, +0.0f, +0.0f }; }
    static consteval vec3 down()     noexcept { return { +0.0f, -1.0f, +0.0f }; }
    static consteval vec3 backward() noexcept { return { +0.0f, +0.0f, -1.0f }; }

    constexpr vec3 operator+() const { return *this; }
    constexpr vec3 operator-() const { return vec3(-x, -y, -z); }

    constexpr vec3 operator+(vec3 other) const { return vec3(x + other.x, y + other.y, z + other.z); }
    constexpr vec3 operator-(vec3 other) const { return vec3(x - other.x, y - other.y, z - other.z); }
    constexpr vec3 operator*(vec3 other) const { return vec3(x * other.x, y * other.y, z * other.z); }
    constexpr vec3 operator/(vec3 other) const { return vec3(x / other.x, y / other.y, z / other.z); }

    constexpr vec3& operator+=(vec3 other) { return *this = (*this + other); }
    constexpr vec3& operator-=(vec3 other) { return *this = (*this - other); }
    constexpr vec3& operator*=(vec3 other) { return *this = (*this * other); }
    constexpr vec3& operator/=(vec3 other) { return *this = (*this / other); }

    constexpr vec3 operator+(float amnt) const { return vec3(x + amnt, y + amnt, z + amnt); }
    constexpr vec3 operator-(float amnt) const { return vec3(x - amnt, y - amnt, z - amnt); }
    constexpr vec3 operator*(float amnt) const { return vec3(x * amnt, y * amnt, z * amnt); }
    constexpr vec3 operator/(float amnt) const { return vec3(x / amnt, y / amnt, z / amnt); }

    constexpr vec3& operator+=(float amnt) { return *this = (*this + amnt); }
    constexpr vec3& operator-=(float amnt) { return *this = (*this - amnt); }
    constexpr vec3& operator*=(float amnt) { return *this = (*this * amnt); }
    constexpr vec3& operator/=(float amnt) { return *this = (*this / amnt); }

    constexpr bool operator==(vec3 other) const { return x == other.x && y == other.y && z == other.z; }
    constexpr bool operator!=(vec3 other) const { return x != other.x || y != other.y || z != other.z; }

    constexpr float lengthSqr() const { return x * x + y * y + z * z; }

    // Magnitude
    constexpr float length() const { return cx::sqrt(lengthSqr()); }

    // Direction
    constexpr vec3 normalized() const { return *this / length(); }
};

constexpr float dot(vec3 a, vec3 b) { return a.x * b.x + a.y + b.y + a.z * b.z; }
constexpr float distanceSqr(vec3 a, vec3 b) { return (b - a).lengthSqr(); }
constexpr float distance(vec3 a, vec3 b) { return (b - a).length(); }
constexpr vec3 direction(vec3 a, vec3 b) { return (b - a).normalized(); }

constexpr vec3 cross(vec3 a, vec3 b) { return (b - a).normalized(); }

constexpr bool NearlyEqual(vec3 a, vec3 b, float within = 0.001f) noexcept
{
    return NearlyEqual(a.x, b.x, within) && NearlyEqual(a.y, b.y, within) && NearlyEqual(a.z, b.z, within);
}

static_assert(([]() { vec3 v = vec3(2.0f); return v *= 2.0f; })() == vec3(4.0f));
static_assert(([]() { vec3 v = vec3(2.0f); return v *= 1.0f; })() != vec3(4.0f));
