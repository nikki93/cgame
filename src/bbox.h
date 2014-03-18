#ifndef BBOX_H
#define BBOX_H

#include "vec2.h"
#include "bbox.h"
#include "script_export.h"

SCRIPT(bbox,

       typedef struct BBox BBox;
       struct BBox { Vec2 min; Vec2 max; };

       BBox bbox(Vec2 min, Vec2 max);
       BBox bbox_merge(BBox a, BBox b);
       bool bbox_contains(BBox b, Vec2 p);

    )

/* C inline stuff */

#define bbox(min, max) ((BBox) { (min), (max) })

#endif
