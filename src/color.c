#include "color.h"

#include <stddef.h>

Color color_black = { 0.0, 0.0, 0.0, 1.0 };
Color color_white = { 1.0, 1.0, 1.0, 1.0 };
Color color_gray  = { 0.5, 0.5, 0.5, 1.0 };
Color color_red   = { 1.0, 0.0, 0.0, 1.0 };
Color color_green = { 0.0, 1.0, 0.0, 1.0 };
Color color_blue  = { 0.0, 0.0, 1.0, 1.0 };
Color color_clear = { 0.0, 0.0, 0.0, 0.0 };

void color_save(Color *c, const char *n, Store *s)
{
    Store *t;

    if (store_child_save(&t, n, s))
    {
        scalar_save(&c->r, "r", t);
        scalar_save(&c->g, "g", t);
        scalar_save(&c->b, "b", t);
        scalar_save(&c->a, "a", t);
    }
}
bool color_load(Color *c, const char *n, Color d, Store *s)
{
    Store *t;

    if (store_child_load(&t, n, s))
    {
        scalar_load(&c->r, "r", 0, t);
        scalar_load(&c->g, "g", 0, t);
        scalar_load(&c->b, "b", 0, t);
        scalar_load(&c->a, "a", 0, t);
    }
    else
        *c = d;
    return t != NULL;
}

#undef color
Color color(Scalar r, Scalar g, Scalar b, Scalar a)
{
    return (Color) { r, g, b, a };
}
