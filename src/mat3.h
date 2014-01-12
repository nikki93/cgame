#ifndef MAT3_H_YVET8M7D
#define MAT3_H_YVET8M7D

#include "script_export.h"
#include "vec2.h"
#include "saveload.h"

SCRIPT(mat3,

        typedef struct Mat3 Mat3;
        struct Mat3 { float m[3][3]; };

        EXPORT Mat3 mat3(float m00, float m01, float m02,
            float m10, float m11, float m12,
            float m20, float m21, float m22);

        EXPORT Mat3 mat3_identity(); /* returns identity matrix */

        /* matrix that applies scale, rot and trans in order */
        EXPORT Mat3 mat3_scaling_rotation_translation(Vec2 scale, float rot,
            Vec2 trans);

        EXPORT Mat3 mat3_inverse(Mat3 m);

      )

void mat3_save(Mat3 *m, Serializer *s);
void mat3_load(Mat3 *m, Deserializer *s);


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

#define mat3_identity() \
    mat3( \
            1.0f, 0.0f, 0.0f, \
            0.0f, 1.0f, 0.0f, \
            0.0f, 0.0f, 1.0f \
        );

#endif

