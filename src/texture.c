#include "texture.h"

#include <string.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <stb_image.h>
#include <sys/stat.h>
#include <time.h>

#include "error.h"
#include "array.h"
#include "console.h"

typedef struct Texture Texture;
struct Texture
{
    char *filename;
    GLuint gl_name;
    int width;
    int height;
    int components;

    time_t last_modified;
};

Array *textures;

/* ------------------------------------------------------------------------- */

static void _flip_image_vertical(unsigned char *data,
                                 unsigned int width, unsigned int height)
{
    unsigned int size, stride, i, j;
    unsigned char *new_data;

    size = width * height * 4;
    stride = sizeof(char) * width * 4;

    new_data = malloc(sizeof(char) * size);
    for (i = 0; i < height; i++)
    {
        j = height - i - 1;
        memcpy(new_data + j * stride, data + i * stride, stride);
    }

    memcpy(data, new_data, size);
    free(new_data);
}

static void _load(Texture *tex)
{
    struct stat st;
    unsigned char *data;

    /* release old GL texture if exists */
    if (tex->gl_name != 0)
        glDeleteTextures(1, &tex->gl_name);

    console_printf("texture: loading texture '%s'\n", tex->filename);

    /* generate GL texture */
    glGenTextures(1, &tex->gl_name);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex->gl_name);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    /* read in texture data */
    data = stbi_load(tex->filename, &tex->width, &tex->height,
                     &tex->components, 0);
    _flip_image_vertical(data, tex->width, tex->height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->width, tex->height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

    /* update modified time */
    stat(tex->filename, &st);
    tex->last_modified = st.st_mtime;
}

static Texture *_find(const char *filename)
{
    Texture *tex;

    array_foreach(tex, textures)
        if (!strcmp(tex->filename, filename))
            return tex;
    return NULL;
}

void texture_load(const char *filename)
{
    Texture *tex;

    if (_find(filename))
        return;

    tex = array_add(textures);
    tex->gl_name = 0;
    tex->filename = malloc(strlen(filename) + 1);
    strcpy(tex->filename, filename);

    _load(tex);
}

void texture_bind(const char *filename)
{
    Texture *tex;

    tex = _find(filename);
    if (tex)
        glBindTexture(GL_TEXTURE_2D, tex->gl_name);
}

Vec2 texture_get_size(const char *filename)
{
    Texture *tex;

    tex = _find(filename);
    error_assert(tex);
    return vec2(tex->width, tex->height);
}

/* ------------------------------------------------------------------------- */

void texture_init()
{
    textures = array_new(Texture);
}
void texture_deinit()
{
    Texture *tex;
    array_foreach(tex, textures)
        free(tex->filename);
    array_free(textures);
}

void texture_update()
{
    struct stat st;
    Texture *tex;

    array_foreach(tex, textures)
    {
        /* outdated? reload */
        stat(tex->filename, &st);
        if (st.st_mtime != tex->last_modified)
            _load(tex);
    }
}
