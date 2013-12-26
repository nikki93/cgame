#ifndef VEC2_H_OWWB2FND
#define VEC2_H_OWWB2FND

/* script_begin */

typedef struct Vec2 Vec2;
struct Vec2 { float x; float y; };

Vec2 vec2(float x, float y);
Vec2 vec2_scalar_mul(Vec2 v, float f);

/* script_end */

/* inline vec2(...) in C */
#define vec2(x, y) ((Vec2) { (x), (y) })

#endif

