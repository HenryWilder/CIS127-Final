#pragma once
#include "vec3.h"

using byte = unsigned char;
struct Color
{
    Color() = default;

    template<std::integral _ITy>
    constexpr Color(_ITy value) :
        r((byte)value), g((byte)value), b((byte)value) {}

    template<std::integral _ITy>
    constexpr Color(_ITy r, _ITy g, _ITy b) :
        r((byte)r), g((byte)g), b((byte)b) {}

    template<std::floating_point _FTy>
    constexpr Color(_FTy value) :
        r((byte)(value * (_FTy)255)), g((byte)(value * (_FTy)255)), b((byte)(value * (_FTy)255)) {}

    template<std::floating_point _FTy>
    constexpr Color(_FTy r, _FTy g, _FTy b) :
        r((byte)(r * (_FTy)255)), g((byte)(g * (_FTy)255)), b((byte)(b * (_FTy)255)) {}

    explicit constexpr Color(vec3 v) :
        r((byte)(v.x * 255)), g((byte)(v.y * 255)), b((byte)(v.z * 255)) {}

    byte r, g, b;

    explicit constexpr operator vec3() const noexcept
    {
        constexpr float conversion = 1.0f / 255.0f;
        return vec3(r * conversion, g * conversion, b * conversion);
    }
};

// Color equivalent of lerp
template<can_interpolate<vec3> _Ty>
constexpr Color mix(Color a, Color b, _Ty t) noexcept
{
    return Color(lerp((vec3)a, (vec3)b, t));
}
