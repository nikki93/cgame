#include "sprite.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <stb_image.h>

#include "array.h"
#include "entitymap.h"
#include "dirs.h"
#include "saveload.h"
#include "transform.h"
#include "camera.h"

typedef struct Sprite Sprite;
struct Sprite
{
    Entity entity;

    Mat3 transform;

    Vec2 cell;
    Vec2 size;
};

static Array *sprites;
static EntityMap *emap; /* map of indices into above */

/* ------------------------------------------------------------------------- */

void sprite_add(Entity ent)
{
    Sprite *sprite;

    if (entitymap_get(emap, ent) >= 0)
        return; /* already has a sprite */

    transform_add(ent);

    sprite = array_add(sprites);
    sprite->entity = ent;
    sprite->cell = vec2(32.0f, 32.0f);
    sprite->size = vec2(32.0f, 32.0f);

    entitymap_set(emap, ent, array_length(sprites) - 1);
}
void sprite_remove(Entity ent)
{
    int i;

    if ((i = entitymap_get(emap, ent)) >= 0)
    {
        if (array_quick_remove(sprites, i))
            entitymap_set(emap, array_get_val(Sprite, sprites, i).entity, i);
        entitymap_set(emap, ent, -1);
    }
}

void sprite_set_cell(Entity ent, Vec2 cell)
{
    int i = entitymap_get(emap, ent);
    assert(i >= 0);
    array_get_val(Sprite, sprites, i).cell = cell;
}
void sprite_set_size(Entity ent, Vec2 size)
{
    int i = entitymap_get(emap, ent);
    assert(i >= 0);
    array_get_val(Sprite, sprites, i).size = size;
}

/* ------------------------------------------------------------------------- */

static GLuint vertex_shader;
static GLuint geometry_shader;
static GLuint fragment_shader;
static GLuint program;

static GLuint vao;
static GLuint sprite_buf_object;

static GLuint atlas_tex;

static void _compile_shader(GLuint shader, const char *filename)
{
    char *file_contents, log[512];
    long input_file_size;
    FILE *input_file;
    GLint status;

    input_file = fopen(filename, "rb");
    fseek(input_file, 0, SEEK_END);
    input_file_size = ftell(input_file);
    rewind(input_file);
    file_contents = malloc((input_file_size + 1) * (sizeof(char)));
    fread(file_contents, sizeof(char), input_file_size, input_file);
    fclose(input_file);
    file_contents[input_file_size] = '\0';

    printf("compiling shader '%s' ...", filename);

    glShaderSource(shader, 1, (const GLchar **) &file_contents, NULL);
    glCompileShader(shader);

    free(file_contents);

    /* log */
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    printf(status ? " successful\n" : " unsuccessful\n");
    glGetShaderInfoLog(shader, 512, NULL, log);
    printf("%s\n", log);
}

/* get pointer offset of 'field' in struct 'type' */
#define poffsetof(type, field) \
    ((void *) (&((type *) 0)->field))

static void _bind_attributes()
{
    /* attribute locations */
    GLuint transform1, transform2, transform3, cell, size;

    transform1 = glGetAttribLocation(program, "transform1");
    glVertexAttribPointer(transform1, 3, GL_FLOAT, GL_FALSE,
            sizeof(Sprite), poffsetof(Sprite, transform.m[0]));
    glEnableVertexAttribArray(transform1);
    transform2 = glGetAttribLocation(program, "transform2");
    glVertexAttribPointer(transform2, 3, GL_FLOAT, GL_FALSE,
            sizeof(Sprite), poffsetof(Sprite, transform.m[1]));
    glEnableVertexAttribArray(transform2);
    transform3 = glGetAttribLocation(program, "transform3");
    glVertexAttribPointer(transform3, 3, GL_FLOAT, GL_FALSE,
            sizeof(Sprite), poffsetof(Sprite, transform.m[2]));
    glEnableVertexAttribArray(transform3);

    cell = glGetAttribLocation(program, "cell");
    glVertexAttribPointer(cell, 2, GL_FLOAT, GL_FALSE,
            sizeof(Sprite), poffsetof(Sprite, cell));
    glEnableVertexAttribArray(cell);

    size = glGetAttribLocation(program, "size");
    glVertexAttribPointer(size, 2, GL_FLOAT, GL_FALSE,
            sizeof(Sprite), poffsetof(Sprite, size));
    glEnableVertexAttribArray(size);
}

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

static void _load_atlases()
{
    int width, height, n; /* n is number of components as returned by
                             stbi_load() -- we don't really care */
    unsigned char *data;

    glGenTextures(1, &atlas_tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlas_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    data = stbi_load(data_path("test/atlas.png"), &width, &height, &n, 0);
    _flip_image_vertical(data, width, height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

    glUniform1i(glGetUniformLocation(program, "tex0"), 0);
    glUniform2f(glGetUniformLocation(program, "atlas_size"), width, height);
}

void sprite_init()
{
    /* initialize array, map */
    sprites = array_new(Sprite);
    emap = entitymap_new(-1);

    /* compile shaders */
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    _compile_shader(vertex_shader, data_path("sprite.vert"));
    geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
    _compile_shader(geometry_shader, data_path("sprite.geom"));
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    _compile_shader(fragment_shader, data_path("sprite.frag"));

    /* link program */
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, geometry_shader);
    glAttachShader(program, fragment_shader);
    glBindFragDataLocation(program, 0, "outColor");
    glLinkProgram(program);
    glUseProgram(program);

    /* make vao */
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    /* make buffer object and bind attributes */
    glGenBuffers(1, &sprite_buf_object);
    glBindBuffer(GL_ARRAY_BUFFER, sprite_buf_object);
    glBufferData(GL_ARRAY_BUFFER, array_length(sprites) * sizeof(Sprite),
            array_get(sprites, 0), GL_STREAM_DRAW);
    _bind_attributes();

    /* load atlas textures */
    _load_atlases();
}

void sprite_deinit()
{
    /* clean up OpenGL stuff */
    glDeleteProgram(program);
    glDeleteShader(fragment_shader);
    glDeleteShader(geometry_shader);
    glDeleteShader(vertex_shader);
    glDeleteBuffers(1, &sprite_buf_object);
    glDeleteVertexArrays(1, &vao);

    /* deinit map, array */
    entitymap_free(emap);
    array_free(sprites);
}

void sprite_update_all()
{
    unsigned int i, n;
    Sprite *sprite;

    for (i = 0; i < array_length(sprites); )
    {
        sprite = array_get(sprites, i);
        if (entity_destroyed(sprite->entity))
            sprite_remove(sprite->entity);
        else
            ++i;
    }

    n = array_length(sprites);
    for (i = 0; i < n; ++i)
    {
        sprite = array_get(sprites, i);
        sprite->transform = transform_get_world_matrix(sprite->entity);
    }
}

void sprite_draw_all()
{
    glBindVertexArray(vao);

    glUniformMatrix3fv(glGetUniformLocation(program, "inverse_view_matrix"),
            1, GL_FALSE,
            (const GLfloat *) camera_get_inverse_view_matrix_ptr());

    glBindBuffer(GL_ARRAY_BUFFER, sprite_buf_object);
    glBufferData(GL_ARRAY_BUFFER, array_length(sprites) * sizeof(Sprite),
            array_get(sprites, 0), GL_STREAM_DRAW);
    glDrawArrays(GL_POINTS, 0, array_length(sprites));
}

void sprite_save_all(FILE *file)
{
    unsigned int i, n;
    Sprite *sprite;

    n = array_length(sprites);

    uint_save(&n, file);
    for (i = 0; i < n; ++i)
    {
        sprite = array_get(sprites, i);
        entity_save(&sprite->entity, file);
        mat3_save(&sprite->transform, file);
        vec2_save(&sprite->cell, file);
        vec2_save(&sprite->size, file);
    }
}
void sprite_load_all(FILE *file)
{
    unsigned int i, n;
    Sprite *sprite;

    uint_load(&n, file);
    array_reset(sprites, n);

    entitymap_clear(emap);

    for (i = 0; i < n; ++i)
    {
        sprite = array_get(sprites, i);
        entity_load(&sprite->entity, file);
        mat3_load(&sprite->transform, file);
        vec2_load(&sprite->cell, file);
        vec2_load(&sprite->size, file);

        entitymap_set(emap, sprite->entity, i);
    }
}

