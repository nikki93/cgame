#ifndef SCALAR_H
#define SCALAR_H

#include <math.h>
#include <float.h>

#include "script_export.h"

SCRIPT(scalar,

       typedef float Scalar;

    )

#ifdef M_PI
#define SCALAR_PI M_PI
#else
#define SCALAR_PI 3.14159265358979323846264338327950288
#endif

#define SCALAR_INFINITY INFINITY

#define SCALAR_EPSILON FLT_EPSILON

#define scalar_cos cosf
#define scalar_sin sinf
#define scalar_atan2 atan2f

#define scalar_sqrt sqrtf

#define scalar_min fminf
#define scalar_max fmaxf

#define scalar_floor floor

#endif

