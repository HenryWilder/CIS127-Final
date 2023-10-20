#pragma once
#include <concepts>
#include "custom_concepts.h"

// The name scheme may seem a little weird. It kinda is.
// I tried to keep in consistent in the sense that math uses snake_case, while higher level functions use PascalCase.
// Imagine it as being where the snake_case is stuff I could imagine the standard library implementing,
// while PascalCase stuff is what I would imagine Unity would have to implement on top of that.
// (camalCase is used where the thing has an OpenGL parallel)

template<std::floating_point _Ty>
constexpr bool NearlyEqual(_Ty a, _Ty b, _Ty within = ((_Ty)1 / (_Ty)1024)) noexcept
{
    _Ty diff = a - b;
    return -within <= diff && diff <= within;
}

// Epic! static assertions can test lambdas!!
static_assert(([]() { return true; })());
static_assert(([]() { return false; })() == false);

template<typename _Ty>
concept comparible = requires(_Ty a, _Ty b)
{
    { a < b } -> std::same_as<bool>;
};

// Returns the larger of two numbers
template<comparible _Ty>
constexpr _Ty max(_Ty a, _Ty b) noexcept
{
    return b < a ? a : b;
}

// Returns the smaller of two numbers
template<comparible _Ty>
constexpr _Ty min(_Ty a, _Ty b) noexcept
{
    return a < b ? a : b;
}

// Keeps a number between two values
template<comparible _Ty>
constexpr _Ty clamp(_Ty x, _Ty min, _Ty max) noexcept
{
    if (x < min) return min;
    if (max < x) return max;
    return x;
}
static_assert(clamp(0.25, 0.0, 1.0) == 0.25);
static_assert(clamp(2.00, 0.0, 1.0) == 1.00);
static_assert(clamp(-1.00, 0.0, 1.0) == 0.00);
static_assert(clamp(2, 0, 1) == 1);

// Clamp 0..1
template<comparible _Ty>
constexpr _Ty saturate(_Ty x) noexcept
{
    if (x < _Ty(0)) return _Ty(0);
    if (_Ty(1) < x) return _Ty(1);
    return x;
}
static_assert(saturate(0.25) == 0.25);
static_assert(saturate(2.00) == 1.00);
static_assert(saturate(-1.00) == 0.00);

template<typename _Ty>
concept basic_arithmetible = requires(_Ty a, _Ty b)
{
    { a + b } -> std::same_as<_Ty>;
    { a - b } -> std::same_as<_Ty>;
};

template<typename _Ty>
concept scalar_arithmetible = requires(_Ty a, _Ty b, float f)
{
    { a + b } -> std::same_as<_Ty>;
    { a - b } -> std::same_as<_Ty>;
    { a * f } -> std::same_as<_Ty>;
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
static_assert(lerp(0.0, 1.0, 0.0) == 0.0);
static_assert(lerp(0.0, 1.0, 1.0) == 1.0);
static_assert(lerp(0.0, 1.0, 0.5) == 0.5);
static_assert(lerp(-1.0, 1.0, 0.5) == 0.0);
static_assert(lerp(0.0, 1.0, 2.0) == 2.0);
static_assert(lerp(1.0, 0.0, 2.0) == -1.0);

template<scalar_arithmetible _Ty>
constexpr _Ty average(_Ty a, _Ty b) noexcept
{
    return (a + b) * 0.5f;
}

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

template<typename _Ty>
concept self_scalar = requires(_Ty x)
{
    { x * x } -> std::same_as<_Ty>;
};

// A value multiplied by itself
template<self_scalar _Ty>
constexpr _Ty sqr(_Ty x)
{
    return x * x;
}


// Allows for the functions to be called explicitly despite sharing std names
namespace cx
{
    template<numeric _Ty>
    constexpr _Ty abs(_Ty x)
    {
        return x < 0 ? -x : x;
    }
    static_assert(abs(1) == 1);
    static_assert(abs(-1) == 1);
    static_assert(abs(0) == 0);

    // Compiletime constexpr square root.
    // Uses Newton Raphson method.
    // Loops instead of using recursion.
    // Not meant to be used negatives. If used for negatives, the result will be the imaginary number's coefficient.
    // sqrt of infinity will loop infinitely.
    template<std::floating_point _Ty>
    constexpr _Ty sqrt(_Ty x)
    {
        x = abs(x);

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
    static_assert(NearlyEqual(sin(0.0f), 0.0f));
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
    static_assert(NearlyEqual(cos(0.0f), 1.0f));
    static_assert(NearlyEqual(cos(PI<float> / 2), 0.0f));
    static_assert(NearlyEqual(cos(PI<float> / 3), 0.5f));
    static_assert(NearlyEqual(cos(PI<float>), -1.0f));
}
