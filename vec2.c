#include "vec2.h"

#include "saveload.h"

Vec2 vec2_scalar_mul(Vec2 v, float f)
{
    return vec2(f * v.x, f * v.y);
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

