#include "scratch.h"

#include <console.h>
#include <stdbool.h>
#include <sys/stat.h>

#include "dirs.h"
#include "script.h"

const char *filename = usr_path("scratch.lua");

bool _exists()
{
    struct stat st;
    return stat(filename, &st) == 0;
}

/* whether modified since last time _modified() was called, false at first */
bool _modified()
{
    struct stat st;
    static struct timespec prev_time;
    bool modified;
    static bool first = true;

    if (stat(filename, &st) != 0)
        return false; /* doesn't exist or error */

    modified = !first && (st.st_mtimespec.tv_sec != prev_time.tv_sec);
    prev_time = st.st_mtimespec;
    first = false;
    return modified;
}

void scratch_run()
{
    console_puts("running scratch buffer");
    script_run_file(filename);
}

void scratch_update()
{
    if (_modified())
        scratch_run();
}
