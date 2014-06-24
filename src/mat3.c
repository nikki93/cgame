#include "mat3.h"

#include <stddef.h>

#include "saveload.h"

Mat3 mat3_mul(Mat3 m, Mat3 n)
{
    return mat3(
        m.m[0][0] * n.m[0][0] + m.m[1][0] * n.m[0][1] + m.m[2][0] * n.m[0][2],
        m.m[0][1] * n.m[0][0] + m.m[1][1] * n.m[0][1] + m.m[2][1] * n.m[0][2],
        m.m[0][2] * n.m[0][0] + m.m[1][2] * n.m[0][1] + m.m[2][2] * n.m[0][2],

        m.m[0][0] * n.m[1][0] + m.m[1][0] * n.m[1][1] + m.m[2][0] * n.m[1][2],
        m.m[0][1] * n.m[1][0] + m.m[1][1] * n.m[1][1] + m.m[2][1] * n.m[1][2],
        m.m[0][2] * n.m[1][0] + m.m[1][2] * n.m[1][1] + m.m[2][2] * n.m[1][2],

        m.m[0][0] * n.m[2][0] + m.m[1][0] * n.m[2][1] + m.m[2][0] * n.m[2][2],
        m.m[0][1] * n.m[2][0] + m.m[1][1] * n.m[2][1] + m.m[2][1] * n.m[2][2],
        m.m[0][2] * n.m[2][0] + m.m[1][2] * n.m[2][1] + m.m[2][2] * n.m[2][2]
        );
}

Mat3 mat3_scaling_rotation_translation(Vec2 scale, Scalar rot, Vec2 trans)
{
    return mat3(scale.x * scalar_cos(rot), scale.x * scalar_sin(rot), 0.0f,
                scale.y * -scalar_sin(rot), scale.y * scalar_cos(rot), 0.0f,
                trans.x, trans.y, 1.0f);
}

Vec2 mat3_get_translation(Mat3 m)
{
    return vec2(m.m[2][0], m.m[2][1]);
}
Scalar mat3_get_rotation(Mat3 m)
{
    return scalar_atan2(m.m[0][1], m.m[0][0]);
}
Vec2 mat3_get_scale(Mat3 m)
{
    return vec2(scalar_sqrt(m.m[0][0] * m.m[0][0] + m.m[0][1] * m.m[0][1]),
                scalar_sqrt(m.m[1][0] * m.m[1][0] + m.m[1][1] * m.m[1][1]));
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

Vec2 mat3_transform(Mat3 m, Vec2 v)
{
    return vec2(
        m.m[0][0] * v.x + m.m[1][0] * v.y + m.m[2][0],
        m.m[0][1] * v.x + m.m[1][1] * v.y + m.m[2][1]
        );
}

void mat3_save(Mat3 *m, const char *n, Serializer *s)
{
    unsigned int i, j;

    serializer_section(n, s)
        for (i = 0; i < 3; ++i)
            for (j = 0; j < 3; ++j)
                scalar_save(&m->m[i][j], NULL, s);
}
bool mat3_load(Mat3 *m, const char *n, Mat3 d, Deserializer *s)
{
    unsigned int i, j;

    deserializer_section(n, s)
        for (i = 0; i < 3; ++i)
            for (j = 0; j < 3; ++j)
                scalar_load(&m->m[i][j], NULL, 0, s);
    else
        for (i = 0; i < 3; ++i)
            for (j = 0; j < 3; ++j)
                m->m[i][j] = d.m[i][j];
    return deserializer_section_found(s);
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
