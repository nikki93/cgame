#include "vec2.h"

#include <math.h>

#include "saveload.h"

Vec2 vec2_add(Vec2 u, Vec2 v)
{
    return vec2(u.x + v.x, u.y + v.y);
}
Vec2 vec2_scalar_mul(Vec2 v, float f)
{
    return vec2(f * v.x, f * v.y);
}

Vec2 vec2_rot(Vec2 v, float rot)
{
    return vec2(v.x * cos(rot) - v.y * sin(rot),
            v.x * sin(rot) + v.y * cos(rot));
}

void vec2_save(Vec2 *v, FILE *file)
{
    scalar_save(&v->x, file);
    scalar_save(&v->y, file);
}
void vec2_load(Vec2 *v, FILE *file)
{
    scalar_load(&v->x, file);
    scalar_load(&v->y, file);
}

#undef vec2
Vec2 vec2(float x, float y)
{
    return (Vec2) { x, y };
}

