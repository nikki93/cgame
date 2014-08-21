#include "fs.h"

#include <stdlib.h>
#include <sys/types.h>
#ifdef _MSC_VER
#include <dirent_win.h>
#else
#include <dirent.h>
#endif

struct Dir
{
    DIR *d;
};

Dir *fs_dir_open(const char *path)
{
    Dir *dir;

    dir = malloc(sizeof(Dir));

    dir->d = opendir(path);
    if (!dir->d)
    {
        free(dir);
        return NULL;
    }

    return dir;
}

const char *fs_dir_next_file(Dir *dir)
{
    struct dirent *de;

    de = readdir(dir->d);
    if (de)
        return de->d_name;
    return NULL;
}

void fs_dir_close(Dir *dir)
{
    closedir(dir->d);
    free(dir);
}
