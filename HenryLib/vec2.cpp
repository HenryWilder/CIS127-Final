#include "vec2.h"
#include <cmath>

vec2 fmod(vec2 v, vec2  div) { return vec2(fmod(v.x, div.x), fmod(v.y, div.y)); }
vec2 fmod(vec2 v, float div) { return vec2(fmod(v.x, div  ), fmod(v.y, div  )); }

vec2 vec2::rand01()
{
    constexpr float RAND_MAX_FLT_INV = 1.0f / RAND_MAX;

    return vec2(
        rand() * RAND_MAX_FLT_INV,
        rand() * RAND_MAX_FLT_INV
    );
}

vec2 vec2::rand_between(vec2 min, vec2 max)
{
    return lerp(min, max, rand01());
}
