#include "bbox.h"

BBox bbox_merge(BBox a, BBox b)
{
    return bbox(vec2(scalar_min(a.min.x, b.min.x),
                     scalar_min(a.min.y, b.min.y)),
                vec2(scalar_max(a.max.x, b.max.x),
                     scalar_max(a.max.y, b.max.y)));
}
BBox bbox_bound(Vec2 a, Vec2 b)
{
    return bbox(vec2(scalar_min(a.x, b.x),
                     scalar_min(a.y, b.y)),
                vec2(scalar_max(a.x, b.x),
                     scalar_max(a.y, b.y)));
}
bool bbox_contains(BBox b, Vec2 p)
{
    return b.min.x <= p.x && p.x <= b.max.x
        && b.min.y <= p.y && p.y <= b.max.y;
}

BBox bbox(Vec2 min, Vec2 max)
{
    BBox bb;
    bb.min = min;
    bb.max = max;
    return bb;
}

BBox bbox_transform(Mat3 m, BBox b)
{
    Vec2 v1, v2, v3, v4;

    v1 = mat3_transform(m, vec2(b.min.x, b.min.y));
    v2 = mat3_transform(m, vec2(b.max.x, b.min.y));
    v3 = mat3_transform(m, vec2(b.max.x, b.max.y));
    v4 = mat3_transform(m, vec2(b.min.x, b.max.y));

    return bbox_merge(bbox_bound(v1, v2), bbox_bound(v3, v4));
}
