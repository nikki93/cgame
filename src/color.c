#include "color.h"

Color color_black = { 0.0, 0.0, 0.0, 1.0 };
Color color_white = { 1.0, 1.0, 1.0, 1.0 };
Color color_red   = { 1.0, 0.0, 0.0, 1.0 };
Color color_green = { 0.0, 1.0, 0.0, 1.0 };
Color color_blue  = { 0.0, 0.0, 1.0, 1.0 };

void color_save(Color *c, Serializer *s)
{
    scalar_save(&c->r, s);
    scalar_save(&c->g, s);
    scalar_save(&c->b, s);
    scalar_save(&c->a, s);
}
void color_load(Color *c, Deserializer *s)
{
    scalar_load(&c->r, s);
    scalar_load(&c->g, s);
    scalar_load(&c->b, s);
    scalar_load(&c->a, s);
}
