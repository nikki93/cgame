#ifndef MAT3_H_YVET8M7D
#define MAT3_H_YVET8M7D

#include "vec2.h"

/* script_begin */

typedef struct Mat3 Mat3;
struct Mat3 { float m[3][3]; };

Mat3 mat3(float m00, float m01, float m02,
        float m10, float m11, float m12,
        float m20, float m21, float m22);

Mat3 mat3_inverse(Mat3 m);

/* matrix that applies scale, rot and trans in order */
Mat3 mat3_scaling_rotation_translation(Vec2 scale, float rot, Vec2 trans);

/* script_end */

void mat3_save(Mat3 *m, FILE *file);
void mat3_load(Mat3 *m, FILE *file);


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

