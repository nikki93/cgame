#ifndef MAT3_H
#define MAT3_H

#include "scalar.h"
#include "script_export.h"
#include "vec2.h"
#include "saveload.h"

SCRIPT(mat3,

       /*
        * stored in column-major order, so that,
        *
        *     m = /                                 \
        *         | m.m[0][0]  m.m[1][0]  m.m[2][0] |
        *         | m.m[0][1]  m.m[1][1]  m.m[2][1] |
        *         | m.m[0][2]  m.m[1][2]  m.m[2][2] |
        *         \                                 /
        */
       typedef struct Mat3 Mat3;
       struct Mat3 { Scalar m[3][3]; };

       EXPORT Mat3 mat3(Scalar m00, Scalar m01, Scalar m02,
                        Scalar m10, Scalar m11, Scalar m12,
                        Scalar m20, Scalar m21, Scalar m22);

       EXPORT Mat3 mat3_identity(); /* returns identity matrix */

       EXPORT Mat3 mat3_mul(Mat3 m, Mat3 n);

       /* matrix that applies scale, rot and trans in order */
       EXPORT Mat3 mat3_scaling_rotation_translation(Vec2 scale, Scalar rot,
                                                     Vec2 trans);

       EXPORT Vec2 mat3_get_translation(Mat3 m);
       EXPORT Scalar mat3_get_rotation(Mat3 m);
       EXPORT Vec2 mat3_get_scale(Mat3 m);

       EXPORT Mat3 mat3_inverse(Mat3 m);

       EXPORT Vec2 mat3_transform(Mat3 m, Vec2 v);

       EXPORT void mat3_save(Mat3 *m, const char *n, Store *s);
       EXPORT bool mat3_load(Mat3 *m, const char *n, Mat3 d, Store *s);

    )

/* C inline stuff */

#define mat3(m00, m01, m02,                     \
             m10, m11, m12,                     \
             m20, m21, m22)                     \
    ((Mat3)                                     \
    {                                           \
        {                                       \
            { (m00), (m01), (m02) },            \
            { (m10), (m11), (m12) },            \
            { (m20), (m21), (m22) }             \
        }                                       \
    })

#define mat3_identity()                         \
    mat3(                                       \
        1.0f, 0.0f, 0.0f,                       \
        0.0f, 1.0f, 0.0f,                       \
        0.0f, 0.0f, 1.0f                        \
        )

#endif

