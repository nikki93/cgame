#include "mat3.h"

#include "saveload.h"

Mat3 mat3_scaling_rotation_translation(Vec2 scale, Scalar rot, Vec2 trans)
{
    return mat3(scale.x * scalar_cos(rot), scale.x * scalar_sin(rot), 0.0f,
                scale.y * -scalar_sin(rot), scale.y * scalar_cos(rot), 0.0f,
                trans.x, trans.y, 1.0f);
}

Mat3 mat3_inverse(Mat3 m)
{
    Scalar det;
    Mat3 inv;

    inv.m[0][0] = m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1];
    inv.m[0][1] = m.m[0][2] * m.m[2][1] - m.m[0][1] * m.m[2][2];
    inv.m[0][2] = m.m[0][1] * m.m[1][2] - m.m[0][2] * m.m[1][1];
    inv.m[1][0] = m.m[1][2] * m.m[2][0] - m.m[1][0] * m.m[2][2];
    inv.m[1][1] = m.m[0][0] * m.m[2][2] - m.m[0][2] * m.m[2][0];
    inv.m[1][2] = m.m[0][2] * m.m[1][0] - m.m[0][0] * m.m[1][2];
    inv.m[2][0] = m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0];
    inv.m[2][1] = m.m[0][1] * m.m[2][0] - m.m[0][0] * m.m[2][1];
    inv.m[2][2] = m.m[0][0] * m.m[1][1] - m.m[0][1] * m.m[1][0];

    det = m.m[0][0] * inv.m[0][0]
        + m.m[0][1] * inv.m[1][0]
        + m.m[0][2] * inv.m[2][0];

    if (det <= 10e-8)
        return inv; /* TODO: figure out what to do if not invertible */

    inv.m[0][0] /= det;
    inv.m[0][1] /= det;
    inv.m[0][2] /= det;
    inv.m[1][0] /= det;
    inv.m[1][1] /= det;
    inv.m[1][2] /= det;
    inv.m[2][0] /= det;
    inv.m[2][1] /= det;
    inv.m[2][2] /= det;

    return inv;
}

void mat3_save(Mat3 *m, Serializer *s)
{
    unsigned int i, j;

    for (i = 0; i < 3; ++i)
        for (j = 0; j < 3; ++j)
            scalar_save(&m->m[i][j], s);
}
void mat3_load(Mat3 *m, Deserializer *s)
{
    unsigned int i, j;

    for (i = 0; i < 3; ++i)
        for (j = 0; j < 3; ++j)
            scalar_load(&m->m[i][j], s);
}

#undef mat3_identity
Mat3 mat3_identity()
{
    return mat3(
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
        );
}

#undef mat3
Mat3 mat3(Scalar m00, Scalar m01, Scalar m02,
          Scalar m10, Scalar m11, Scalar m12,
          Scalar m20, Scalar m21, Scalar m22)
{
    return (Mat3)
    {
        {
            { m00, m01, m02 },
            { m10, m11, m12 },
            { m20, m21, m22 }
        }
    };
}

