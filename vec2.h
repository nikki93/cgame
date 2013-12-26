#ifndef VEC2_H_OWWB2FND
#define VEC2_H_OWWB2FND

/* script_begin */

typedef union Vec2 Vec2;
union Vec2
{
    float v[2];
    struct { float x; float y; };
};

Vec2 vec2(float x, float y);

/* script_end */

/* inline vec2(...) in C */
#define vec2(x, y) ((Vec2) { (x), (y) })

#endif

