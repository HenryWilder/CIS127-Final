#include "vec2.h"
#include <cmath>

vec2 fmod(vec2 v, vec2  div) { return vec2(fmod(v.x, div.x), fmod(v.y, div.y)); }
vec2 fmod(vec2 v, float div) { return vec2(fmod(v.x, div  ), fmod(v.y, div  )); }
