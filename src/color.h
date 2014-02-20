#ifndef COLOR_H
#define COLOR_H

#include "scalar.h"

typedef struct Color Color;
struct Color { Scalar r, g, b, a; };

extern Color color_black;

#define color(r, g, b, a) ((Color) { (r), (g), (b), (a) })

#endif
