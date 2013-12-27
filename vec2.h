#ifndef VEC2_H_OWWB2FND
#define VEC2_H_OWWB2FND

#include <stdio.h>

/* script_begin */

typedef struct Vec2 Vec2;
struct Vec2 { float x; float y; };

Vec2 vec2(float x, float y);
Vec2 vec2_scalar_mul(Vec2 v, float f);

/* script_end */

void vec2_save(Vec2 *v, FILE *file);
void vec2_load(Vec2 *v, FILE *file);


/* C inline stuff */

#define vec2(x, y) ((Vec2) { (x), (y) })

#endif

