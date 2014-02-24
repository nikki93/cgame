#include "scratch.h"

#include "dirs.h"
#include "script.h"

const char *filename = usr_path("scratch.lua");

void scratch_run()
{
    script_run_file(filename);
}

void scratch_update()
{
}
