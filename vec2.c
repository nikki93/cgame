#include "vec2.h"

Vec2 vec2_scalar_mul(Vec2 v, float f)
{
    return vec2(f * v.x, f * v.y);
}

#undef vec2
Vec2 vec2(float x, float y)
{
    return (Vec2) { x, y };
}

