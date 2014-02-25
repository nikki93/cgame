#include "vec2.h"

#include "saveload.h"

Vec2 vec2_zero = { 0.0, 0.0 };

Vec2 vec2_add(Vec2 u, Vec2 v)
{
    return vec2(u.x + v.x, u.y + v.y);
}
Vec2 vec2_sub(Vec2 u, Vec2 v)
{
    return vec2(u.x - v.x, u.y - v.y);
}
Vec2 vec2_div(Vec2 u, Vec2 v)
{
    return vec2(u.x / v.x, u.y / v.y);
}
Vec2 vec2_scalar_mul(Vec2 v, Scalar f)
{
    return vec2(v.x * f, v.y * f);
}
Vec2 vec2_scalar_div(Vec2 v, Scalar f)
{
    return vec2(v.x / f, v.y / f);
}

Scalar vec2_len(Vec2 v)
{
    return scalar_sqrt(v.x * v.x + v.y * v.y);
}
Vec2 vec2_normalize(Vec2 v)
{
    if (v.x == 0 && v.y == 0)
        return v;
    return vec2_scalar_div(v, vec2_len(v));
}

Vec2 vec2_rot(Vec2 v, Scalar rot)
{
    return vec2(v.x * scalar_cos(rot) - v.y * scalar_sin(rot),
                v.x * scalar_sin(rot) + v.y * scalar_cos(rot));
}

void vec2_save(Vec2 *v, Serializer *s)
{
    scalar_save(&v->x, s);
    scalar_save(&v->y, s);
}
void vec2_load(Vec2 *v, Deserializer *s)
{
    scalar_load(&v->x, s);
    scalar_load(&v->y, s);
}

#undef vec2
Vec2 vec2(Scalar x, Scalar y)
{
    return (Vec2) { x, y };
}

