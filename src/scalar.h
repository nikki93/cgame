#ifndef SCALAR_H_WBZWRG0U
#define SCALAR_H_WBZWRG0U

#include <math.h>

#include "script_export.h"

SCRIPT(scalar,

       typedef float Scalar;

    )

#ifdef M_PI
#define SCALAR_PI M_PI
#else
#define SCALAR_PI 3.14159265358979323846264338327950288
#endif

#define scalar_cos cosf
#define scalar_sin sinf

#endif

