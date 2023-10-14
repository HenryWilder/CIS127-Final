#pragma once
#include <concepts>

// Epic! static assertions can test lambdas!!
static_assert(([]() { return true; })());
static_assert(([]() { return false; })() == false);

template<typename _Ty>
concept numeric = std::is_floating_point_v<_Ty> || std::is_integral_v<_Ty>;

template<typename _Ty>
concept comparible = requires(_Ty a, _Ty b)
{
    { a < b } -> std::same_as<bool>;
};

// Keeps a number between two values
template<comparible _Ty>
constexpr _Ty clamp(_Ty x, _Ty min, _Ty max) noexcept
{
    if (x < min) return min;
    if (max < x) return max;
    return x;
}
static_assert(clamp( 0.25, 0.0, 1.0) == 0.25);
static_assert(clamp( 2.00, 0.0, 1.0) == 1.00);
static_assert(clamp(-1.00, 0.0, 1.0) == 0.00);
static_assert(clamp( 2,    0,   1  ) == 1   );

// Clamp 0..1
template<comparible _Ty>
constexpr _Ty saturate(_Ty x) noexcept
{
    if (x < _Ty(0)) return _Ty(0);
    if (_Ty(1) < x) return _Ty(1);
    return x;
}
static_assert(saturate( 0.25) == 0.25);
static_assert(saturate( 2.00) == 1.00);
static_assert(saturate(-1.00) == 0.00);

template<typename _Ty>
concept basic_arithmetible = requires(_Ty a, _Ty b)
{
    { a + b } -> std::same_as<_Ty>;
    { a - b } -> std::same_as<_Ty>;
};

template<typename _TTy, typename _Ty>
concept can_interpolate = requires(_Ty a, _TTy t)
{
    { a * t } -> std::same_as<_Ty>;
};

// Linear interpolate
template<basic_arithmetible _Ty, can_interpolate<_Ty> _TTy>
constexpr _Ty lerp(_Ty a, _Ty b, _TTy t) noexcept
{
    return a + (b - a) * t;
}
static_assert(lerp( 0.0, 1.0, 0.0) ==  0.0);
static_assert(lerp( 0.0, 1.0, 1.0) ==  1.0);
static_assert(lerp( 0.0, 1.0, 0.5) ==  0.5);
static_assert(lerp(-1.0, 1.0, 0.5) ==  0.0);
static_assert(lerp( 0.0, 1.0, 2.0) ==  2.0);
static_assert(lerp( 1.0, 0.0, 2.0) == -1.0);

// Quadratic interpolate
template<basic_arithmetible _Ty, can_interpolate<_Ty> _TTy>
constexpr _Ty qerp(_Ty a, _Ty b, _Ty c, _TTy t) noexcept
{
    return lerp(lerp(a, b), lerp(b, c), t);
}

// Cubic interpolate
template<basic_arithmetible _Ty, can_interpolate<_Ty> _TTy>
constexpr _Ty cerp(_Ty a, _Ty b, _Ty c, _Ty d, _TTy t) noexcept
{
    return qerp(lerp(a, b), lerp(b, c), lerp(c, d), t);
}


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

    constexpr bool NearlyEqual(vec2 other, float within = 0.001f) const
    {
        float xDiff = x - other.x;
        float yDiff = y - other.y;
        return
            (-within <= xDiff && xDiff <= within) &&
            (-within <= yDiff && yDiff <= within);
    }

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
};

static_assert(lerp(vec2(0.0f), vec2(1.0f), 0.5f).NearlyEqual(vec2(0.5f)));
static_assert(lerp(vec2(-1.0f, 1.0f), vec2(1.0f, 2.0f), 0.5f).NearlyEqual(vec2(0.0f, 1.5f)));


struct vec3
{
    vec3() = default;

    constexpr vec3(float x) :
        x(x), y(x), z(x) {}

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
    static constexpr vec3 forward()  { return vec3(+0.0f, +0.0f, +1.0f); }
    static constexpr vec3 left()     { return vec3(-1.0f, +0.0f, +0.0f); }
    static constexpr vec3 down()     { return vec3(+0.0f, -1.0f, +0.0f); }
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
};

static_assert(([]() {
        vec3 v = vec3(2.0f);
        return v *= 2.0f;
    })() == vec3(4.0f));

static_assert(([]() {
        vec3 v = vec3(2.0f);
        return v *= 1.0f;
    })() != vec3(4.0f));


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

    constexpr Color(vec3 v) :
        r((byte)(v.x * 255)), g((byte)(v.y * 255)), b((byte)(v.z * 255)) {}

    byte r, g, b;

    explicit constexpr operator vec3() const
    {
        constexpr float conversion = 1.0f / 255;
        return vec3(r * conversion, g * conversion, b * conversion);
    }
};

// Color equivalent of lerp
template<can_interpolate<vec3> _Ty>
constexpr Color mix(Color a, Color b, _Ty t)
{
    return Color(lerp((vec3)a, (vec3)b, t));
}

// A square is a rect with width == height
struct irect
{
    int xmin, ymin, xmax, ymax;
};

// A square is a rect with width == height
struct rect
{
    float xmin, ymin, xmax, ymax;
};

struct triangle
{

};

struct quad
{

};

// A circle is an oval with rad1 == rad2
struct oval
{

};
