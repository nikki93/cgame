#ifndef MAT3_H_YVET8M7D
#define MAT3_H_YVET8M7D

#include "vec2.h"

/* NOTE: matrices are stored in column-major order */

/* script_begin */

typedef struct Mat3 Mat3;
struct Mat3 { float m[3][3]; };

Mat3 mat3(float m00, float m01, float m02,
        float m10, float m11, float m12,
        float m20, float m21, float m22);

Mat3 mat3_translation_rotation(Vec2 trans, float rot);

/* script_end */

/* C inline stuff */

#define mat3(m00, m01, m02, \
        m10, m11, m12, \
        m20, m21, m22) \
    ((Mat3) \
    { \
        { \
            { (m00), (m01), (m02) }, \
            { (m10), (m11), (m12) }, \
            { (m20), (m21), (m22) } \
        } \
    });

#endif

