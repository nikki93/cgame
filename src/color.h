#ifndef COLOR_H
#define COLOR_H

#include "scalar.h"

SCRIPT(color,

       typedef struct Color Color;
       struct Color { Scalar r, g, b, a; };

       EXPORT extern Color color_black;

    )

#define color(r, g, b, a) ((Color) { (r), (g), (b), (a) })

#endif
