#ifndef COLOR_H
#define COLOR_H

#include "scalar.h"
#include "script_export.h"
#include "saveload.h"

SCRIPT(color,

       typedef struct Color Color;
       struct Color { Scalar r, g, b, a; };

       EXPORT Color color(Scalar r, Scalar g, Scalar b, Scalar a);
       EXPORT Color color_opaque(Scalar r, Scalar g, Scalar b);
       EXPORT extern Color color_black;
       EXPORT extern Color color_white;
       EXPORT extern Color color_gray;
       EXPORT extern Color color_red;
       EXPORT extern Color color_green;
       EXPORT extern Color color_blue;
       EXPORT extern Color color_clear; /* zero alpha */

       EXPORT void color_save(Color *c, const char *n, Store *s);
       EXPORT bool color_load(Color *c, const char *n, Color d, Store *s);

    )

#define color(r, g, b, a) ((Color) { (r), (g), (b), (a) })
#define color_opaque(r, g, b, a) color(r, g, b, 1)

#endif
