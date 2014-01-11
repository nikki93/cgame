#ifndef VEC2_H_OWWB2FND
#define VEC2_H_OWWB2FND

#include "saveload.h"
#include "script.h"

SCRIPT(vec2,

        typedef struct Vec2 Vec2;
        struct Vec2 { float x; float y; };

        EXPORT Vec2 vec2(float x, float y);

        EXPORT Vec2 vec2_add(Vec2 u, Vec2 v);
        EXPORT Vec2 vec2_scalar_mul(Vec2 v, float f);

        EXPORT Vec2 vec2_rot(Vec2 v, float rot);

        void vec2_save(Vec2 *v, Serializer *s);
        void vec2_load(Vec2 *v, Deserializer *s);
      )

/* C inline stuff */

#define vec2(x, y) ((Vec2) { (x), (y) })

#endif

