#include "fs.h"

#include <stdlib.h>
#include <sys/types.h>
#ifndef _MSC_VER
#include <dirent.h>
#endif

struct Dir
{
#ifndef _MSC_VER
    DIR *d;
#else
    void *d;
#endif
};

Dir *fs_dir_open(const char *path)
{
#ifdef _MSC_VER
    return NULL;
#else
    Dir *dir;

    dir = malloc(sizeof(Dir));

    dir->d = opendir(path);
    if (!dir->d)
    {
        free(dir);
        return NULL;
    }

    return dir;
#endif
}

const char *fs_dir_next_file(Dir *dir)
{
#ifdef _MSC_VER
    return NULL;
#else
    struct dirent *de;

    de = readdir(dir->d);
    if (de)
        return de->d_name;
    return NULL;
#endif
}

void fs_dir_close(Dir *dir)
{
#ifndef _MSC_VER
    closedir(dir->d);
    free(dir);
#endif
}
