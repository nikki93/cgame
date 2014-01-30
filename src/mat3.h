#ifndef MAT3_H_YVET8M7D
#define MAT3_H_YVET8M7D

#include "scalar.h"
#include "script_export.h"
#include "vec2.h"
#include "saveload.h"

SCRIPT(mat3,

       typedef struct Mat3 Mat3;
       struct Mat3 { Scalar m[3][3]; };

       EXPORT Mat3 mat3(Scalar m00, Scalar m01, Scalar m02,
                        Scalar m10, Scalar m11, Scalar m12,
                        Scalar m20, Scalar m21, Scalar m22);

       EXPORT Mat3 mat3_identity(); /* returns identity matrix */

       /* matrix that applies scale, rot and trans in order */
       EXPORT Mat3 mat3_scaling_rotation_translation(Vec2 scale, Scalar rot,
                                                     Vec2 trans);

       EXPORT Mat3 mat3_inverse(Mat3 m);

       EXPORT void mat3_save(Mat3 *m, Serializer *s);
       EXPORT void mat3_load(Mat3 *m, Deserializer *s);

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
    });

#define mat3_identity()                         \
    mat3(                                       \
        1.0f, 0.0f, 0.0f,                       \
        0.0f, 1.0f, 0.0f,                       \
        0.0f, 0.0f, 1.0f                        \
        );

#endif

