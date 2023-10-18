#pragma once

#include "cxmath.h"
#include "ivec2.h"
#include "irect.h"
#include "vec2.h"
#include "rect.h"
#include "vec3.h"
#include "shapes.h"
#include "color.h"

template<typename _Enum>
concept enumeration = std::is_enum_v<_Enum>;

template<typename _Ty>
concept series = requires
{
    typename _Ty::value_type;
    { _Ty::_Size }-> std::convertible_to<int>;
};

template<typename _Ty, int _Size>
concept series_of = series<_Ty> && _Ty::_Size == _Size;

template<typename _Ty, int _Dim>
concept vec_of = series<_Ty> && _Ty::_Size == _Dim;

