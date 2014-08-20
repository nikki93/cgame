#ifndef FS_H
#define FS_H

#include "script_export.h"

SCRIPT(fs,

       /* remember to *_close(...) when done to free resources! */

       typedef struct Dir Dir;

       EXPORT Dir *fs_dir_open(const char *path);
       EXPORT const char *fs_dir_next_file(Dir *dir); /* NULL after last file */
       EXPORT void fs_dir_close(Dir *dir);

    )

#endif
