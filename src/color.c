#include "color.h"

Color color_black = { 0.0, 0.0, 0.0, 1.0 };
Color color_white = { 1.0, 1.0, 1.0, 1.0 };
Color color_gray  = { 0.5, 0.5, 0.5, 1.0 };
Color color_red   = { 1.0, 0.0, 0.0, 1.0 };
Color color_green = { 0.0, 1.0, 0.0, 1.0 };
Color color_blue  = { 0.0, 0.0, 1.0, 1.0 };
Color color_clear = { 0.0, 0.0, 0.0, 0.0 };

void color_save(Color *c, const char *n, Serializer *s)
{
    serializer_section(n, s)
    {
        scalar_save(&c->r, "r", s);
        scalar_save(&c->g, "g", s);
        scalar_save(&c->b, "b", s);
        scalar_save(&c->a, "a", s);
    }
}
bool color_load(Color *c, const char *n, Color d, Deserializer *s)
{
    deserializer_section(n, s)
    {
        scalar_load(&c->r, "r", 0, s);
        scalar_load(&c->g, "g", 0, s);
        scalar_load(&c->b, "b", 0, s);
        scalar_load(&c->a, "a", 1, s);
    }
    else
        *c = d;
    return deserializer_section_found(s);
}

#undef color
Color color(Scalar r, Scalar g, Scalar b, Scalar a)
{
    return (Color) { r, g, b, a };
}
