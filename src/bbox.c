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

#undef bbox
BBox bbox(Vec2 min, Vec2 max)
{
    return (BBox) { min, max };
}

