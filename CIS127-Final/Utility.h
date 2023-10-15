#pragma once
#include <concepts>
#include <array>
#include <limits>
#include <sal.h>

// The name scheme may seem a little weird. It kinda is.
// I tried to keep in consistent in the sense that math uses snake_case, while higher level functions use PascalCase.
// Imagine it as being where the snake_case is stuff I could imagine the standard library implementing,
// while PascalCase stuff is what I would imagine Unity would have to implement on top of that.
// (camalCase is used where the thing has an OpenGL parallel)

template<typename _Ty>
concept numeric = std::is_floating_point_v<_Ty> || std::is_integral_v<_Ty>;

template<std::floating_point _Ty = float> constexpr _Ty PI = (_Ty)3.141592653589793238462643;
template<std::floating_point _Ty = float> constexpr _Ty TAU = PI<_Ty> * 2; // 2pi

template<std::floating_point _Ty>
constexpr _Ty deg_to_rad(_Ty angle)
{
    return angle * TAU<_Ty>;
}

template<std::floating_point _Ty>
constexpr _Ty rad_to_deg(_Ty angle)
{
    constexpr _Ty converter = 1 / TAU<_Ty>;
    return angle * converter;
}

// constexpr math
namespace cxmath
{
    template<numeric _Ty>
    constexpr _Ty abs(_Ty x)
    {
        return x < 0 ? -x : x;
    }
    static_assert(abs(1) == 1);
    static_assert(abs(-1) == 1);
    static_assert(abs(0) == 0);

    // Compiletime constexpr square root. Uses Newton Raphson method. Loops instead of using recursion.
    // You are not meant to use this for negatives. But if you do, the result will be the imaginary number's coefficient.
    // Infinity * infinity is still infinity, therefore the square root of infinity is negative infinity.
    template<std::floating_point _Ty>
    constexpr _Ty sqrt(_In_range_(>= , 0) _Ty x)
    {
        x = x >= 0 ? x : -x;

        if (x == std::numeric_limits<_Ty>::infinity())
        {
            return std::numeric_limits<_Ty>::infinity();
        }

        _Ty curr = x;
        _Ty prev = 0;
        while (curr != prev)
        {
            prev = curr;
            curr = (curr + x / curr) / 2;
        }
        return curr;
    }
    static_assert(sqrt(4.0f) == 2.0f);
    static_assert(sqrt(16.0f) == 4.0f);
    static_assert(sqrt(0.25f) == 0.5f);
    static_assert(sqrt(0.0f) == 0.0f);
    static_assert(sqrt(1.0f) == 1.0f);
    static_assert(sqrt(-1.0f) == 1);
    static_assert(sqrt(std::numeric_limits<float>::infinity()) == std::numeric_limits<float>::infinity());
    static_assert(sqrt(-std::numeric_limits<float>::infinity()) == std::numeric_limits<float>::infinity());

    // Compiletime constexpr sine.
    template<std::floating_point _Ty>
    constexpr _Ty sin(_Ty x)
    {
        _Ty t = x;
        _Ty prev = 0;
        _Ty sine = t;
        int n = 1;
        const _Ty xnegx = -x * x;
        while (sine != prev)
        {
            prev = sine;
            sine += t *= xnegx / ((2 * n + 1) * 2 * n);
            ++n;
        }
        return sine;
    }
    static_assert(sin(0.0f) == 0.0f);
    static_assert(NearlyEqual(sin(PI<float> / 2), 1.0f));
    static_assert(NearlyEqual(sin(PI<float> / 6), 0.5f));
    static_assert(NearlyEqual(sin(PI<float>), 0.0f));

    // Compiletime constexpr cosine.
    template<std::floating_point _Ty>
    constexpr _Ty cos(_Ty x)
    {
        _Ty t = 1;
        _Ty prev = 0;
        _Ty cosine = t;
        int n = 1;
        const _Ty xnegx = -x * x;
        while (cosine != prev)
        {
            prev = cosine;
            int part = 2 * n - 1;
            cosine += t *= xnegx / ((2 * n - 1) * 2 * n);
            ++n;
        }
        return cosine;
    }
    static_assert(cos(0.0f) == 1.0f);
    static_assert(NearlyEqual(cos(PI<float> / 2), 0.0f));
    static_assert(NearlyEqual(cos(PI<float> / 3), 0.5f));
    static_assert(NearlyEqual(cos(PI<float>), -1.0f));
}
using namespace cxmath; // If std ever shadows these functions, they can still be accessed explicitly.

// Epic! static assertions can test lambdas!!
static_assert(([]() { return true; })());
static_assert(([]() { return false; })() == false);

template<typename _Ty>
concept comparible = requires(_Ty a, _Ty b)
{
    { a < b } -> std::same_as<bool>;
};

template<std::floating_point _Ty>
constexpr bool NearlyEqual(_Ty a, _Ty b, _Ty within = (_Ty)0.001) noexcept
{
    _Ty diff = a - b;
    return -within <= diff && diff <= within;
}

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

#if 0
template<size_t _Size>
class nbool
{
public:
    nbool() = default;

    template<std::same_as<bool> ..._Args, typename std::enable_if<sizeof...(_Args) == _Size, bool>::type = false>
    constexpr nbool(_Args... args) noexcept
    {
        this->values = { args... };
    }

    constexpr nbool(bool fill)
    {
        for (bool& v : values)
        {
            v = fill;
        }
    }

    constexpr bool& operator[](size_t index) { return values[index]; }
    constexpr bool operator[](size_t index) const { return values[index]; }

    constexpr nbool& operator&=(nbool other) noexcept { for (size_t i = 0; i < _Size; ++i) { values[i] = values[i] && other[i]; } return *this; }
    constexpr nbool& operator|=(nbool other) noexcept { for (size_t i = 0; i < _Size; ++i) { values[i] = values[i] || other[i]; } return *this; }
    constexpr nbool& operator^=(nbool other) noexcept { for (size_t i = 0; i < _Size; ++i) { values[i] = values[i] != other[i]; } return *this; }

    constexpr nbool operator&(nbool other) const noexcept { nbool result = *this; result &= other; return result; }
    constexpr nbool operator|(nbool other) const noexcept { nbool result = *this; result |= other; return result; }
    constexpr nbool operator^(nbool other) const noexcept { nbool result = *this; result ^= other; return result; }
    constexpr nbool operator~() const noexcept { nbool result = *this; for (size_t i = 0; i < _Size; ++i) { result[i] = !result[i]; } return result; }

    constexpr bool operator==(nbool other) const noexcept { for (size_t i = 0; i < _Size; ++i) { if (values[i] != other[i]) return false; } return true; }
    constexpr bool operator!=(nbool other) const noexcept { for (size_t i = 0; i < _Size; ++i) { if (values[i] != other[i]) return true; } return false; }

    constexpr bool none() const noexcept { for (bool x : values) { if (x) return false; } return true; }
    constexpr bool all() const noexcept { for (bool x : values) { if (!x) return false; } return true; }
    constexpr bool any() const noexcept { for (bool x : values) { if (x) return true; } return false; }

    constexpr size_t size() const noexcept { return _Size; }

private:
    std::array<bool, _Size> values;
};
using bool2 = nbool<2>;
using bool3 = nbool<3>;

static_assert(bool2(false, true)[0] == false);
static_assert(bool2(false, true)[1] == true);

static_assert(!nbool<4>(true, true, false, true).all());
static_assert(nbool<4>(true, true, true, true).all());
static_assert(nbool<4>(false, false, true, false).any());
static_assert(!nbool<4>(false, false, false, false).any());
static_assert(nbool<4>(false, false, false, false).none());
static_assert(!nbool<4>(false, false, true, false).none());
#endif


struct ivec2
{
    ivec2() = default;

    constexpr ivec2(int x) :
        x(x), y(x) {}

    constexpr ivec2(int x, int y) :
        x(x), y(y) {}

    int x, y;

    static constexpr ivec2 zero()     noexcept { return ivec2(+0); }
    static constexpr ivec2 one()      noexcept { return ivec2(+1); }
    static constexpr ivec2 negative() noexcept { return ivec2(-1); }
    static constexpr ivec2 right()    noexcept { return ivec2(+1,+0); }
    static constexpr ivec2 up()       noexcept { return ivec2(+0,+1); }
    static constexpr ivec2 left()     noexcept { return ivec2(-1,+0); }
    static constexpr ivec2 down()     noexcept { return ivec2(+0,-1); }

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
    constexpr float length() const { return sqrt(lengthSqr()); }

    // Direction
    constexpr vec2 normal() const { return *this / length(); }

    // angle should be in radians
    constexpr vec2 FromAngle(float angle) const { return vec2(cos(angle), sin(angle)); }
};

static constexpr float dot(vec2 a, vec2 b) { return a.x * b.x + a.y + b.y; }
static constexpr float det(vec2 a, vec2 b) { return a.x * b.y - a.y + b.x; }
static constexpr float distanceSqr(vec2 a, vec2 b) { return ; }
static constexpr float distance(vec2 a, vec2 b) { return ; }
static constexpr float direction(vec2 a, vec2 b) { return ; }

constexpr bool NearlyEqual(vec2 a, vec2 b, float within = 0.001f) noexcept
{
    return NearlyEqual(a.x, b.x, within) && NearlyEqual(a.y, b.y, within);
}

static_assert(NearlyEqual(lerp(vec2(0.0f), vec2(1.0f), 0.5f), vec2(0.5f)));
static_assert(NearlyEqual(lerp(vec2(-1.0f, 1.0f), vec2(1.0f, 2.0f), 0.5f), vec2(0.0f, 1.5f)));


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

    static constexpr vec3 zero()     noexcept { return vec3(+0.0f); }
    static constexpr vec3 one()      noexcept { return vec3(+1.0f); }
    static constexpr vec3 negative() noexcept { return vec3(-1.0f); }
    static constexpr vec3 right()    noexcept { return vec3(+1.0f, +0.0f, +0.0f); }
    static constexpr vec3 up()       noexcept { return vec3(+0.0f, +1.0f, +0.0f); }
    static constexpr vec3 forward()  noexcept { return vec3(+0.0f, +0.0f, +1.0f); }
    static constexpr vec3 left()     noexcept { return vec3(-1.0f, +0.0f, +0.0f); }
    static constexpr vec3 down()     noexcept { return vec3(+0.0f, -1.0f, +0.0f); }
    static constexpr vec3 backward() noexcept { return vec3(+0.0f, +0.0f, -1.0f); }

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
    constexpr float length() const { return sqrt(lengthSqr()); }

    // Direction
    constexpr vec3 normal() const { return *this / length(); }
};

constexpr float dot(vec3 a, vec3 b) { return a.x * b.x + a.y + b.y + a.z * b.z; }
constexpr float distanceSqr(vec3 a, vec3 b) {}
constexpr float distance(vec3 a, vec3 b) { return sqrt(distanceSqr(a, b)); }
constexpr vec3 direction(vec3 a, vec3 b) { return (b - a).normal(); }

constexpr vec3 cross(vec3 a, vec3 b) { return (b - a).normal(); }

constexpr bool NearlyEqual(vec3 a, vec3 b, float within = 0.001f) noexcept
{
    return NearlyEqual(a.x, b.x, within) && NearlyEqual(a.y, b.y, within) && NearlyEqual(a.z, b.z, within);
}

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

struct irect
{
    irect() = default;

    constexpr irect(int xmin, int ymin, int xmax, int ymax) :
        xmin(xmin), ymin(ymin), xmax(xmax), ymax(ymax) {}

    int xmin, ymin, xmax, ymax;

    static constexpr irect FromMinAndMax(ivec2 min, ivec2 max) noexcept
    {
        return irect(min.x, min.y, max.x, max.y);
    }

    static constexpr irect FromPositionAndSize(ivec2 pos, ivec2 size) noexcept
    {
        return irect(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
    }

    static constexpr irect FromPositionAndSize(ivec2 pos, int size) noexcept
    {
        return irect(pos.x, pos.y, pos.x + size, pos.y + size);
    }
};

struct rect
{
    rect() = default;

    constexpr rect(float x, float y, float size) :
        xmin(x), ymin(y), xmax(x + size), ymax(y + size) {}

    constexpr rect(vec2 position, float size) :
        xmin(position.x), ymin(position.y), xmax(position.x + size), ymax(position.y + size) {}

    constexpr rect(float xmin, float ymin, float xmax, float ymax) :
        xmin(xmin), ymin(ymin), xmax(xmax), ymax(ymax) {}

    float xmin, ymin, xmax, ymax;

    static constexpr irect FromMinAndMax(vec2 min, vec2 max) noexcept
    {
        return irect(min.x, min.y, max.x, max.y);
    }

    static constexpr irect FromPositionAndSize(vec2 pos, vec2 size) noexcept
    {
        return irect(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
    }

    static constexpr irect FromPositionAndSize(vec2 pos, float size) noexcept
    {
        return irect(pos.x, pos.y, pos.x + size, pos.y + size);
    }
};

struct triangle
{
    triangle() = default;

    constexpr triangle(vec2 p1, vec2 p2, vec2 p3) :
        points{ p1, p2, p3 } {}

    vec2 points[3];
};

struct quad
{
    quad() = default;

    constexpr quad(vec2 p1, vec2 p2, vec2 p3, vec2 p4) :
        points{ p1, p2, p3, p4 } {}

    vec2 points[4];
};

struct circle
{
    circle() = default;

    constexpr circle(vec2 position, float radius) :
        position(position), radius(radius) {}

    vec2 position;
    float radius;
};

struct oval
{
    oval() = default;

    constexpr oval(vec2 position, float r1, float r2) :
        position(position), r1(r1), r2(r2) {}

    vec2 position;
    float r1, r2;
};
