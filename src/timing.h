#ifndef TIME_H
#define TIME_H

#include <stdbool.h>

#include "scalar.h"
#include "script_export.h"
#include "saveload.h"

SCRIPT(timing,

       EXPORT extern Scalar timing_dt;
       EXPORT extern Scalar timing_true_dt; /* actual delta time,
                                               unaffected by scale/pause */

       EXPORT void timing_set_scale(Scalar s);
       EXPORT Scalar timing_get_scale();

       EXPORT void timing_set_paused(bool p); /* pause sets scale to 0 and
                                                 restores it on resume */
       EXPORT bool timing_get_paused();

    )

void timing_update();
void timing_save_all(Serializer *s);
void timing_load_all(Deserializer *s);

#endif

