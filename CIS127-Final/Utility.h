#pragma once
#include <concepts>

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

template<typename _Ty1, typename _Ty2>
constexpr _Ty1 lerp(_Ty1 a, _Ty1 b, _Ty2 t) noexcept
{
    return a + (b - a) * t;
}
static_assert(lerp( 0.0, 1.0, 0.0) ==  0.0);
static_assert(lerp( 0.0, 1.0, 1.0) ==  1.0);
static_assert(lerp( 0.0, 1.0, 0.5) ==  0.5);
static_assert(lerp(-1.0, 1.0, 0.5) ==  0.0);
static_assert(lerp( 0.0, 1.0, 2.0) ==  2.0);
static_assert(lerp( 1.0, 0.0, 2.0) == -1.0);


struct ivec2
{
    ivec2() = default;

    constexpr ivec2(int x) :
        x(x), y(x) {}

    constexpr ivec2(int x, int y) :
        x(x), y(y) {}

    int x, y;

    static constexpr ivec2 zero()     { return ivec2(+0); }
    static constexpr ivec2 one()      { return ivec2(+1); }
    static constexpr ivec2 negative() { return ivec2(-1); }
    static constexpr ivec2 right()    { return ivec2(+1,+0); }
    static constexpr ivec2 up()       { return ivec2(+0,+1); }
    static constexpr ivec2 left()     { return ivec2(-1,+0); }
    static constexpr ivec2 down()     { return ivec2(+0,-1); }
};

constexpr ivec2 operator+(ivec2 v) { return v; }
constexpr ivec2 operator-(ivec2 v) { return ivec2(-v.x, -v.y); }

constexpr ivec2 operator+(ivec2 a, vec2 b) { return ivec2(a.x + b.x, a.y + b.y); }
constexpr ivec2 operator-(ivec2 a, vec2 b) { return ivec2(a.x - b.x, a.y - b.y); }
constexpr ivec2 operator*(ivec2 a, vec2 b) { return ivec2(a.x * b.x, a.y * b.y); }
constexpr ivec2 operator/(ivec2 a, vec2 b) { return ivec2(a.x / b.x, a.y / b.y); }

constexpr ivec2& operator+=(ivec2& a, ivec2 b) { return a = (a + b); }
constexpr ivec2& operator-=(ivec2& a, ivec2 b) { return a = (a - b); }
constexpr ivec2& operator*=(ivec2& a, ivec2 b) { return a = (a * b); }
constexpr ivec2& operator/=(ivec2& a, ivec2 b) { return a = (a / b); }

constexpr ivec2 operator+(ivec2 v, float amnt) { return ivec2(v.x + amnt, v.y + amnt); }
constexpr ivec2 operator-(ivec2 v, float amnt) { return ivec2(v.x - amnt, v.y - amnt); }
constexpr ivec2 operator*(ivec2 v, float amnt) { return ivec2(v.x * amnt, v.y * amnt); }
constexpr ivec2 operator/(ivec2 v, float amnt) { return ivec2(v.x / amnt, v.y / amnt); }

constexpr ivec2& operator+=(ivec2& v, float amnt) { return v = (v + amnt); }
constexpr ivec2& operator-=(ivec2& v, float amnt) { return v = (v - amnt); }
constexpr ivec2& operator*=(ivec2& v, float amnt) { return v = (v * amnt); }
constexpr ivec2& operator/=(ivec2& v, float amnt) { return v = (v / amnt); }

constexpr ivec2 operator+(float amnt, ivec2 v) { return ivec2(amnt + v.x, amnt + v.y); }
constexpr ivec2 operator-(float amnt, ivec2 v) { return ivec2(amnt - v.x, amnt - v.y); }
constexpr ivec2 operator*(float amnt, ivec2 v) { return ivec2(amnt * v.x, amnt * v.y); }
constexpr ivec2 operator/(float amnt, ivec2 v) { return ivec2(amnt / v.x, amnt / v.y); }

constexpr bool operator==(ivec2 a, ivec2 b) { return a.x == b.x && a.y == b.y; }
constexpr bool operator!=(ivec2 a, ivec2 b) { return a.x != b.x || a.y != b.y; }

struct vec2
{
    vec2() = default;

    constexpr vec2(float x) :
        x(x), y(x) {}

    constexpr vec2(float x, float y) :
        x(x), y(y) {}

    float x, y;

    static constexpr vec2 zero()     { return vec2(+0.0f); }
    static constexpr vec2 one()      { return vec2(+1.0f); }
    static constexpr vec2 negative() { return vec2(-1.0f); }
    static constexpr vec2 right()    { return vec2(+1.0f, +0.0f); }
    static constexpr vec2 up()       { return vec2(+0.0f, +1.0f); }
    static constexpr vec2 left()     { return vec2(-1.0f, +0.0f); }
    static constexpr vec2 down()     { return vec2(+0.0f, -1.0f); }

    static constexpr bool NearlyEqual(vec2 a, vec2 b, float within = 0.001f)
    {
        float xDiff = a.x - b.x;
        float yDiff = a.y - b.y;
        return
            (-within <= xDiff && xDiff <= within) &&
            (-within <= yDiff && yDiff <= within);
    }
};

constexpr vec2 operator+(vec2 v) { return v; }
constexpr vec2 operator-(vec2 v) { return vec2(-v.x, -v.y); }

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

constexpr bool operator==(vec2 a, vec2 b) { return a.x == b.x && a.y == b.y; }
constexpr bool operator!=(vec2 a, vec2 b) { return a.x == b.x && a.y == b.y; }

static_assert(vec2::NearlyEqual(lerp(vec2(0.0f), vec2(1.0f), 0.5f), vec2(0.5f)));
static_assert(vec2::NearlyEqual(lerp(vec2(-1.0f, 1.0f), vec2(1.0f, 2.0f), 0.5f), vec2(0.0f, 1.5f)));


struct vec3
{
    vec3() = default;

    constexpr vec3(float x) :
        x(x), y(x) {}

    // Two-element initialization of a vec3 isn't very intuitive, so I'm disallowing it to prevent misbehavior.
    vec3(float x, float y) = delete;

    constexpr vec3(vec2 xy, float z) :
        x(xy.x), y(xy.y), z(z) {}

    constexpr vec3(float x, float y, float z) :
        x(x), y(y), z(z) {}

    float x, y, z;

    static constexpr vec3 zero()     { return vec3(+0.0f); }
    static constexpr vec3 one()      { return vec3(+1.0f); }
    static constexpr vec3 negative() { return vec3(-1.0f); }
    static constexpr vec3 right()    { return vec3(+1.0f, +0.0f, +0.0f); }
    static constexpr vec3 up()       { return vec3(+0.0f, +1.0f, +0.0f); }
    static constexpr vec3 left()     { return vec3(-1.0f, +0.0f, +0.0f); }
    static constexpr vec3 down()     { return vec3(+0.0f, -1.0f, +0.0f); }
    static constexpr vec3 forward()  { return vec3(+0.0f, +0.0f, +1.0f); }
    static constexpr vec3 backward() { return vec3(+0.0f, +0.0f, -1.0f); }

    static constexpr bool NearlyEqual(vec3 a, vec3 b, float within = 0.001f)
    {
        float xDiff = a.x - b.x;
        float yDiff = a.y - b.y;
        float zDiff = a.z - b.z;
        return
            (-within <= xDiff && xDiff <= within) &&
            (-within <= yDiff && yDiff <= within) &&
            (-within <= zDiff && zDiff <= within);
    }
};

constexpr vec3 operator+(vec3 v) { return v; }
constexpr vec3 operator-(vec3 v) { return vec3(-v.x, -v.y, -v.z); }

constexpr vec3 operator+(vec3 a, vec3 b) { return vec3(a.x + b.x, a.y + b.y, a.z + b.z); }
constexpr vec3 operator-(vec3 a, vec3 b) { return vec3(a.x - b.x, a.y - b.y, a.z - b.z); }
constexpr vec3 operator*(vec3 a, vec3 b) { return vec3(a.x * b.x, a.y * b.y, a.z * b.z); }
constexpr vec3 operator/(vec3 a, vec3 b) { return vec3(a.x / b.x, a.y / b.y, a.z / b.z); }

constexpr vec3& operator+=(vec3& a, vec3 b) { return a = (a + b); }
constexpr vec3& operator-=(vec3& a, vec3 b) { return a = (a - b); }
constexpr vec3& operator*=(vec3& a, vec3 b) { return a = (a * b); }
constexpr vec3& operator/=(vec3& a, vec3 b) { return a = (a / b); }

constexpr vec3 operator+(vec3 v, float amnt) { return vec3(v.x + amnt, v.y + amnt, v.z + amnt); }
constexpr vec3 operator-(vec3 v, float amnt) { return vec3(v.x - amnt, v.y - amnt, v.z - amnt); }
constexpr vec3 operator*(vec3 v, float amnt) { return vec3(v.x * amnt, v.y * amnt, v.z * amnt); }
constexpr vec3 operator/(vec3 v, float amnt) { return vec3(v.x / amnt, v.y / amnt, v.z / amnt); }

constexpr vec3& operator+=(vec3& v, float amnt) { return v = (v + amnt); }
constexpr vec3& operator-=(vec3& v, float amnt) { return v = (v - amnt); }
constexpr vec3& operator*=(vec3& v, float amnt) { return v = (v * amnt); }
constexpr vec3& operator/=(vec3& v, float amnt) { return v = (v / amnt); }

constexpr vec3 operator+(float amnt, vec3 v) { return vec3(amnt + v.x, amnt + v.y, amnt + v.z); }
constexpr vec3 operator-(float amnt, vec3 v) { return vec3(amnt - v.x, amnt - v.y, amnt - v.z); }
constexpr vec3 operator*(float amnt, vec3 v) { return vec3(amnt * v.x, amnt * v.y, amnt * v.z); }
constexpr vec3 operator/(float amnt, vec3 v) { return vec3(amnt / v.x, amnt / v.y, amnt / v.z); }

constexpr bool operator==(vec3 a, vec3 b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
constexpr bool operator!=(vec3 a, vec3 b) { return a.x == b.x && a.y == b.y && a.z == b.z; }


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
        r((byte)(value * 255)), g((byte)(value * 255)), b((byte)(value * 255)) {}

    template<std::floating_point _FTy>
    constexpr Color(_FTy r, _FTy g, _FTy b) :
        r((byte)(r * 255)), g((byte)(g * 255)), b((byte)(b * 255)) {}

    constexpr Color(vec3 v) :
        r((byte)(v.x * 255)), g((byte)(v.y * 255)), b((byte)(v.z * 255)) {}

    byte r, g, b;

    constexpr operator vec3() const
    {
        constexpr float conversion = 1.0f / 255;
        return vec3(r * conversion, g * conversion, b * conversion);
    }
};

template<std::floating_point _Ty>
constexpr Color mix(Color a, Color b, _Ty t)
{
    return Color(lerp(a.r, b.r, t), lerp(a.g, b.g, t), lerp(a.b, b.b, t));
}
