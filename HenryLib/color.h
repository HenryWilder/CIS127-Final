#pragma once
#include "vec3.h"

using byte = unsigned char;
constexpr byte operator""_u(char ch) { return (byte)ch; }

struct Color
{
    using value_type = byte;
    static constexpr int _Size = 3;

    Color() = default;

    constexpr Color(int gray) :
        r((byte)gray), g((byte)gray), b((byte)gray) {}

    constexpr Color(int r, int g, int b) :
        r((byte)r), g((byte)g), b((byte)b) {}

    constexpr Color(byte gray) :
        r(gray), g(gray), b(gray) {}

    constexpr Color(byte r, byte g, byte b) :
        r(r), g(g), b(b) {}

    constexpr Color(float gray) :
        r((byte)(gray * 255.0f)), g((byte)(gray * 255.0f)), b((byte)(gray * 255.0f)) {}

    constexpr Color(float r, float g, float b) :
        r((byte)(r * 255.0f)), g((byte)(g * 255.0f)), b((byte)(b * 255.0f)) {}

    byte r, g, b;

    // Explicit conversions between color and vec3

    explicit constexpr Color(vec3 v) :
        r((byte)(v.x * 255.0f)), g((byte)(v.y * 255.0f)), b((byte)(v.z * 255.0f)) {}

    explicit constexpr operator vec3() const noexcept
    {
        constexpr float conversion = 1.0f / 255.0f;
        return vec3(r * conversion, g * conversion, b * conversion);
    }

    constexpr float ToGray() const
    {
        constexpr vec3 conversion = vec3(0.299f, 0.587f, 0.114f);
        return dot((vec3)(*this), conversion);
    }
};

constexpr Color operator""_rgb(_In_range_(0x000000ull, 0xFFFFFFull) unsigned long long hexCode)
{
    // It is intentional that the type cast narrows to a byte. This saves having to mask.
    return Color(
        (byte)(hexCode >> 020ull),
        (byte)(hexCode >> 010ull),
        (byte)(hexCode >> 000ull));
}
static_assert(([]() { Color test = 0x000000_rgb; return test.r == '\x00'_u && test.g == '\x00'_u && test.b == '\x00'_u; })());
static_assert(([]() { Color test = 0xFF0000_rgb; return test.r == '\xFF'_u && test.g == '\x00'_u && test.b == '\x00'_u; })());
static_assert(([]() { Color test = 0x00FF00_rgb; return test.r == '\x00'_u && test.g == '\xFF'_u && test.b == '\x00'_u; })());
static_assert(([]() { Color test = 0x0000FF_rgb; return test.r == '\x00'_u && test.g == '\x00'_u && test.b == '\xFF'_u; })());
static_assert(([]() { Color test = 0xFFFFFF_rgb; return test.r == '\xFF'_u && test.g == '\xFF'_u && test.b == '\xFF'_u; })());

// Color equivalent of lerp
template<can_interpolate<vec3> _Ty>
constexpr Color mix(Color a, Color b, _Ty t) noexcept
{
    return Color(lerp((vec3)a, (vec3)b, t));
}
