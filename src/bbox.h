#ifndef BBOX_H
#define BBOX_H

#include "vec2.h"
#include "bbox.h"
#include "script_export.h"

SCRIPT(bbox,

       typedef struct BBox BBox;
       struct BBox { Vec2 min; Vec2 max; };

       EXPORT BBox bbox(Vec2 min, Vec2 max);
       EXPORT BBox bbox_bound(Vec2 a, Vec2 b);
       EXPORT BBox bbox_merge(BBox a, BBox b);
       EXPORT bool bbox_contains(BBox b, Vec2 p);

    )

#endif
