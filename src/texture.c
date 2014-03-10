#include "texture.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <stb_image.h>

#include "dirs.h"
#include "array.h"

typedef struct Texture Texture;
struct Texture
{
    char *filename;
    GLuint gl_name;
    int width;
    int height;
    int components;
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

void texture_load(const char *filename)
{
    unsigned char *data;
    Texture *tex;

    /* add Texture element */
    tex = array_add(textures);
    tex->filename = malloc(strlen(filename) + 1);
    strcpy(tex->filename, filename);

    /* generate GL texture */
    glGenTextures(1, &tex->gl_name);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex->gl_name);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    /* read in texture data */
    data = stbi_load(filename, &tex->width, &tex->height,
                     &tex->components, 0);
    _flip_image_vertical(data, tex->width, tex->height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->width, tex->height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

Texture *_find(const char *filename)
{
    Texture *tex, *end;

    for (tex = array_begin(textures), end = array_end(textures);
         tex != end; ++tex)
        if (!strcmp(tex->filename, filename))
            return tex;
    return NULL;
}

void texture_bind(const char *filename)
{
    Texture *tex;

    tex = _find(filename);
    if (tex)
        glBindTexture(GL_TEXTURE_2D, tex->gl_name);
}

int texture_get_width(const char *filename)
{
    Texture *tex;

    tex = _find(filename);
    assert(tex);
    return tex->width;
}
int texture_get_height(const char *filename)
{
    Texture *tex;

    tex = _find(filename);
    assert(tex);
    return tex->height;
}

/* ------------------------------------------------------------------------- */

void texture_init()
{
    textures = array_new(Texture);
}
void texture_deinit()
{
    Texture *tex, *end;
    for (tex = array_begin(textures), end = array_end(textures);
         tex != end; ++tex)
        free(tex->filename);
    array_free(textures);
}





