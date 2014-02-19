#ifndef TIME_H
#define TIME_H

#include "scalar.h"
#include "script_export.h"
#include "saveload.h"

SCRIPT(timing,

       EXPORT extern Scalar timing_dt;
       EXPORT void timing_set_scale(Scalar s);
       EXPORT Scalar timing_get_scale();

    )

void timing_update();
void timing_save_all(Serializer *s);
void timing_load_all(Deserializer *s);

#endif

