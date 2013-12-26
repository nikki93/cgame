#include "mat3.h"

#include <math.h>

Mat3 mat3_scaling_rotation_translation(Vec2 scale, float rot, Vec2 trans)
{
    return mat3(scale.x * cos(rot), scale.x * sin(rot), 0.0f,
            scale.y * -sin(rot), scale.y * cos(rot), 0.0f,
            trans.x, trans.y, 1.0f);
}

#undef mat3
Mat3 mat3(float m00, float m01, float m02,
        float m10, float m11, float m12,
        float m20, float m21, float m22)
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

