#pragma once
#include "ivec2.h"
#include "irect.h"
#include "vec2.h"
#include "vec3.h"
#include "rect.h"

// Helper concepts

template<typename _Vec>
concept any_vec =
    std::same_as<_Vec, ivec2> ||
    std::same_as<_Vec, vec2> ||
    std::same_as<_Vec, vec3>;

template<typename _Rect>
concept any_rect =
    std::same_as<_Rect, irect> ||
    std::same_as<_Rect, rect>;

template<typename _Ty>
concept vec_or_rect =
    any_rect<_Ty> ||
    any_vec<_Ty>;

template<typename _Vec>
constexpr bool is_dimensional_vec_v = any_vec<_Vec> && requires { { _Vec::_Dim } -> std::convertible_to<unsigned>; };

template<typename _Vec, unsigned _Dim> concept vec_of_dim = is_dimensional_vec_v<_Vec> && _Vec::_Dim == _Dim;
template<typename _Vec, unsigned _Dim> concept vec_ge_dim = is_dimensional_vec_v<_Vec> && _Vec::_Dim >= _Dim;
template<typename _Vec, unsigned _Dim> concept vec_le_dim = is_dimensional_vec_v<_Vec> && _Vec::_Dim <= _Dim;

// Declarations

template<vec_or_rect _Ty> constexpr _Ty zero;
template<vec_or_rect _Ty> constexpr _Ty one;
template<vec_or_rect _Ty> constexpr _Ty unit = one<_Ty>; // Alias for 'one'
template<vec_or_rect _Ty> constexpr _Ty negative;
template<vec_ge_dim<2> _Vec> constexpr _Vec left;
template<vec_ge_dim<2> _Vec> constexpr _Vec right;
template<vec_ge_dim<2> _Vec> constexpr _Vec up;
template<vec_ge_dim<2> _Vec> constexpr _Vec down;
template<vec_ge_dim<3> _Vec> constexpr _Vec forward;
template<vec_ge_dim<3> _Vec> constexpr _Vec backward;

// Definitions

// Vectors

template<> constexpr ivec2 zero     <ivec2> = ivec2(+0, +0);
template<> constexpr ivec2 one      <ivec2> = ivec2(+1, +1);
template<> constexpr ivec2 negative <ivec2> = ivec2(-1, -1);
template<> constexpr ivec2 right    <ivec2> = ivec2(+1, +0);
template<> constexpr ivec2 up       <ivec2> = ivec2(+0, +1);
template<> constexpr ivec2 left     <ivec2> = ivec2(-1, +0);
template<> constexpr ivec2 down     <ivec2> = ivec2(+0, -1);

template<> constexpr vec2 zero     <vec2> = vec2(+0.0f, +0.0f);
template<> constexpr vec2 one      <vec2> = vec2(+1.0f, +1.0f);
template<> constexpr vec2 negative <vec2> = vec2(-1.0f, -1.0f);
template<> constexpr vec2 right    <vec2> = vec2(+1.0f, +0.0f);
template<> constexpr vec2 up       <vec2> = vec2(+0.0f, +1.0f);
template<> constexpr vec2 left     <vec2> = vec2(-1.0f, +0.0f);
template<> constexpr vec2 down     <vec2> = vec2(+0.0f, -1.0f);

template<> constexpr vec3 zero     <vec3> = vec3(+0.0f, +0.0f, +0.0f);
template<> constexpr vec3 one      <vec3> = vec3(+1.0f, +1.0f, +1.0f);
template<> constexpr vec3 negative <vec3> = vec3(-1.0f, -1.0f, -1.0f);
template<> constexpr vec3 right    <vec3> = vec3(+1.0f, +0.0f, +0.0f);
template<> constexpr vec3 up       <vec3> = vec3(+0.0f, +1.0f, +0.0f);
template<> constexpr vec3 forward  <vec3> = vec3(+0.0f, +0.0f, +1.0f);
template<> constexpr vec3 left     <vec3> = vec3(-1.0f, +0.0f, +0.0f);
template<> constexpr vec3 down     <vec3> = vec3(+0.0f, -1.0f, +0.0f);
template<> constexpr vec3 backward <vec3> = vec3(+0.0f, +0.0f, -1.0f);

// Rectangles

template<> constexpr irect zero     <irect> = irect(+0, +0, +0, +0);
template<> constexpr irect one      <irect> = irect(+0, +0, +1, +1);
template<> constexpr irect negative <irect> = irect(+0, +0, -1, -1);

template<> constexpr rect zero     <rect> = rect(+0.0f, +0.0f, +0.0f, +0.0f);
template<> constexpr rect one      <rect> = rect(+0.0f, +0.0f, +1.0f, +1.0f);
template<> constexpr rect negative <rect> = rect(+0.0f, +0.0f, -1.0f, -1.0f);

// Colors

constexpr byte operator""_u(char ch) { return (byte)ch; }

constexpr Color WHITE = 0xFFFFFF_rgb;
constexpr Color GRAY  = 0x808080_rgb;
constexpr Color BLACK = 0x000000_rgb;
constexpr Color RED   = 0xFF0000_rgb;
constexpr Color GREEN = 0x00FF00_rgb;
constexpr Color BLUE  = 0x0000FF_rgb;
