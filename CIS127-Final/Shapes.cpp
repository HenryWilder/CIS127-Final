#include "Shapes.h"
#include <cmath>

float Length(vec2 a)
{
    return sqrtf(Dot(a, a));
}

float Distance(vec2 a, vec2 b)
{
    return sqrtf(DistanceSqr(a.x, b.x));
}

vec2 Normalize(vec2 v)
{
    return v / Length(v);
}

vec2 ProjectPointToLine(vec2 lineStart, vec2 lineEnd, vec2 pt)
{
    vec2 direction = Normalize(lineEnd - lineStart);
    float f = Dot(direction, pt - lineStart);
    return lineStart + direction * f;
}
