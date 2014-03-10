#include "scratch.h"

#include <console.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <time.h>

#include "dirs.h"
#include "script.h"

static const char *filename = usr_path("scratch.lua");

bool _exists()
{
    struct stat st;
    return stat(filename, &st) == 0;
}

/* whether modified since last time _modified() was called, false at first */
bool _modified()
{
    struct stat st;
    static time_t prev_time = 0;
    bool modified;
    static bool first = true;

    if (stat(filename, &st) != 0)
    {
        first = false;
        return false; /* doesn't exist or error */
    }

    modified = !first && st.st_mtime != prev_time;
    prev_time = st.st_mtime;
    first = false;
    return modified;
}

void scratch_run()
{
    script_run_file(filename);
}

void scratch_update()
{
    if (_modified())
        scratch_run();
}
