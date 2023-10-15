#pragma once
#include "cxmath.h"

struct ivec2
{
    using value_type = int;
    static constexpr int _Size = 2;

    ivec2() = default;

    constexpr ivec2(int x) :
        x(x), y(x) {}

    constexpr ivec2(int x, int y) :
        x(x), y(y) {}

    value_type x, y;

    static constexpr ivec2 zero()     noexcept { return ivec2(+0); }
    static constexpr ivec2 one()      noexcept { return ivec2(+1); }
    static constexpr ivec2 negative() noexcept { return ivec2(-1); }
    static constexpr ivec2 right()    noexcept { return ivec2(+1, +0); }
    static constexpr ivec2 up()       noexcept { return ivec2(+0, +1); }
    static constexpr ivec2 left()     noexcept { return ivec2(-1, +0); }
    static constexpr ivec2 down()     noexcept { return ivec2(+0, -1); }

    constexpr ivec2 operator+() const { return *this; }
    constexpr ivec2 operator-() const { return ivec2(-x, -y); }

    constexpr ivec2 operator+(ivec2 other) const { return ivec2(x + other.x, y + other.y); }
    constexpr ivec2 operator-(ivec2 other) const { return ivec2(x - other.x, y - other.y); }
    constexpr ivec2 operator*(ivec2 other) const { return ivec2(x * other.x, y * other.y); }
    constexpr ivec2 operator/(ivec2 other) const { return ivec2(x / other.x, y / other.y); }

    constexpr ivec2& operator+=(ivec2 other) { return *this = (*this + other); }
    constexpr ivec2& operator-=(ivec2 other) { return *this = (*this - other); }
    constexpr ivec2& operator*=(ivec2 other) { return *this = (*this * other); }
    constexpr ivec2& operator/=(ivec2 other) { return *this = (*this / other); }

    constexpr ivec2 operator+(int amnt) const { return ivec2(x + amnt, y + amnt); }
    constexpr ivec2 operator-(int amnt) const { return ivec2(x - amnt, y - amnt); }
    template<std::integral _ITy> constexpr ivec2 operator*(_ITy amnt) const { return ivec2(x * amnt, y * amnt); }
    template<std::integral _ITy> constexpr ivec2 operator/(_ITy amnt) const { return ivec2(x / amnt, y / amnt); }
    template<std::floating_point _FTy> constexpr ivec2 operator*(_FTy amnt) const { return ivec2((int)((_FTy)x * amnt), (int)((_FTy)y * amnt)); }
    template<std::floating_point _FTy> constexpr ivec2 operator/(_FTy amnt) const { return ivec2((int)((_FTy)x / amnt), (int)((_FTy)y / amnt)); }

    constexpr ivec2& operator+=(int amnt) { return *this = (*this + amnt); }
    constexpr ivec2& operator-=(int amnt) { return *this = (*this - amnt); }
    template<numeric _Ty> constexpr ivec2& operator*=(_Ty amnt) { return *this = (*this * amnt); }
    template<numeric _Ty> constexpr ivec2& operator/=(_Ty amnt) { return *this = (*this / amnt); }

    constexpr bool operator==(ivec2 other) const { return x == other.x && y == other.y; }
    constexpr bool operator!=(ivec2 other) const { return x != other.x || y != other.y; }
};

constexpr ivec2 operator+(int amnt, ivec2 v) { return ivec2(amnt + v.x, amnt + v.y); }
constexpr ivec2 operator-(int amnt, ivec2 v) { return ivec2(amnt - v.x, amnt - v.y); }
constexpr ivec2 operator*(int amnt, ivec2 v) { return ivec2(amnt * v.x, amnt * v.y); }
constexpr ivec2 operator/(int amnt, ivec2 v) { return ivec2(amnt / v.x, amnt / v.y); }
