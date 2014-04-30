#ifndef FS_H
#define FS_H

#include "script_export.h"

SCRIPT(fs,

       /* remember to *_close(...) when done to free resources! */

       typedef struct Dir Dir;

       Dir *fs_dir_open(const char *path);
       const char *fs_dir_next_file(Dir *dir); /* NULL after last file */
       void fs_dir_close(Dir *dir);

    )

#endif
