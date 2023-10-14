#pragma once

template<typename _Ty>
constexpr _Ty clamp(_Ty x, _Ty min, _Ty max) noexcept
{
    if (x < min) return min;
    if (x > max) return max;
    return x;
}
static_assert(clamp( 0.25, 0.0, 1.0) == 0.25);
static_assert(clamp( 2.00, 0.0, 1.0) == 1.00);
static_assert(clamp(-1.00, 0.0, 1.0) == 0.00);
static_assert(clamp( 2,    0,   1  ) == 1   );

template<typename _Ty>
constexpr _Ty saturate(_Ty x) noexcept
{
    if (x < _Ty(0)) return _Ty(0);
    if (x > _Ty(1)) return _Ty(1);
    return x;
}
static_assert(saturate( 0.25) == 0.25);
static_assert(saturate( 2.00) == 1.00);
static_assert(saturate(-1.00) == 0.00);

template<typename _ABTy, typename _TTy>
constexpr _ABTy lerp(_ABTy a, _ABTy b, _TTy t) noexcept
{
    return a + (b - a) * t;
}
static_assert(lerp( 0.0, 1.0, 0.0) ==  0.0);
static_assert(lerp( 0.0, 1.0, 1.0) ==  1.0);
static_assert(lerp( 0.0, 1.0, 0.5) ==  0.5);
static_assert(lerp(-1.0, 1.0, 0.5) ==  0.0);
static_assert(lerp( 0.0, 1.0, 2.0) ==  2.0);
static_assert(lerp( 1.0, 0.0, 2.0) == -1.0);

struct Color
{
    unsigned char r, g, b;
};

struct ivec2
{
    ivec2() = default;

    constexpr ivec2(int x) :
        x(x), y(x) {}

    constexpr ivec2(int x, int y) :
        x(x), y(y) {}

    int x, y;

    static constexpr ivec2 zero()  { return ivec2(+0); }
    static constexpr ivec2 one()   { return ivec2(+1); }
    static constexpr ivec2 right() { return ivec2(+1,+0); }
    static constexpr ivec2 up()    { return ivec2(+0,+1); }
    static constexpr ivec2 left()  { return ivec2(-1,+0); }
    static constexpr ivec2 down()  { return ivec2(+0,-1); }
};

struct vec2
{
    vec2() = default;

    constexpr vec2(float x) :
        x(x), y(x) {}

    constexpr vec2(float x, float y) :
        x(x), y(y) {}

    float x, y;

    static constexpr ivec2 zero()  { return ivec2(+0.0f); }
    static constexpr ivec2 one()   { return ivec2(+1.0f); }
    static constexpr ivec2 right() { return ivec2(+1.0f, +0.0f); }
    static constexpr ivec2 up()    { return ivec2(+0.0f, +1.0f); }
    static constexpr ivec2 left()  { return ivec2(-1.0f, +0.0f); }
    static constexpr ivec2 down()  { return ivec2(+0.0f, -1.0f); }
};

constexpr vec2 operator+(vec2 a, vec2 b) { return vec2(a.x + b.x, a.y + b.y); }
constexpr vec2 operator-(vec2 a, vec2 b) { return vec2(a.x - b.x, a.y - b.y); }
constexpr vec2 operator*(vec2 a, vec2 b) { return vec2(a.x * b.x, a.y * b.y); }
constexpr vec2 operator/(vec2 a, vec2 b) { return vec2(a.x / b.x, a.y / b.y); }

constexpr vec2& operator+=(vec2& a, vec2 b) { return a = (a + b); }
constexpr vec2& operator-=(vec2& a, vec2 b) { return a = (a - b); }
constexpr vec2& operator*=(vec2& a, vec2 b) { return a = (a * b); }
constexpr vec2& operator/=(vec2& a, vec2 b) { return a = (a / b); }

constexpr vec2 operator+(vec2 v, float amnt) { return vec2(v.x + amnt, v.y + amnt); }
constexpr vec2 operator-(vec2 v, float amnt) { return vec2(v.x - amnt, v.y - amnt); }
constexpr vec2 operator*(vec2 v, float amnt) { return vec2(v.x * amnt, v.y * amnt); }
constexpr vec2 operator/(vec2 v, float amnt) { return vec2(v.x / amnt, v.y / amnt); }

constexpr vec2& operator+=(vec2& v, float amnt) { return v = (v + amnt); }
constexpr vec2& operator-=(vec2& v, float amnt) { return v = (v - amnt); }
constexpr vec2& operator*=(vec2& v, float amnt) { return v = (v * amnt); }
constexpr vec2& operator/=(vec2& v, float amnt) { return v = (v / amnt); }

constexpr vec2 operator+(float amnt, vec2 v) { return vec2(amnt + v.x, amnt + v.y); }
constexpr vec2 operator-(float amnt, vec2 v) { return vec2(amnt - v.x, amnt - v.y); }
constexpr vec2 operator*(float amnt, vec2 v) { return vec2(amnt * v.x, amnt * v.y); }
constexpr vec2 operator/(float amnt, vec2 v) { return vec2(amnt / v.x, amnt / v.y); }
