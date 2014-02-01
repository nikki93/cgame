#include "sprite.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <stb_image.h>

#include "entitypool.h"
#include "dirs.h"
#include "saveload.h"
#include "transform.h"
#include "camera.h"
#include "texture.h"

typedef struct Sprite Sprite;
struct Sprite
{
    EntityPoolElem pool_elem;

    Mat3 transform;

    Vec2 cell;
    Vec2 size;
};

static EntityPool *pool;

/* ------------------------------------------------------------------------- */

void sprite_add(Entity ent)
{
    Sprite *sprite;

    if (entitypool_get(pool, ent))
        return; /* already has a sprite */

    transform_add(ent);

    sprite = entitypool_add(pool, ent);
    sprite->cell = vec2(32.0f, 32.0f);
    sprite->size = vec2(32.0f, 32.0f);
}
void sprite_remove(Entity ent)
{
    entitypool_remove(pool, ent);
}

void sprite_set_cell(Entity ent, Vec2 cell)
{
    Sprite *sprite = entitypool_get(pool, ent);
    assert(sprite);
    sprite->cell = cell;
}
void sprite_set_size(Entity ent, Vec2 size)
{
    Sprite *sprite = entitypool_get(pool, ent);
    assert(sprite);
    sprite->size = size;
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

    printf("sprite: compiling shader '%s' ...", filename);

    glShaderSource(shader, 1, (const GLchar **) &file_contents, NULL);
    glCompileShader(shader);

    free(file_contents);

    /* log */
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    printf(status ? " successful\n" : " unsuccessful\n");
    glGetShaderInfoLog(shader, 512, NULL, log);
    printf("%s", log);
}

/* get pointer offset of 'field' in struct 'type' */
#define poffsetof(type, field)                  \
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
    texture_load(data_path("test/atlas.png"));
    glUniform1i(glGetUniformLocation(program, "tex0"), 0);
    glUniform2f(glGetUniformLocation(program, "atlas_size"),
                texture_get_width(data_path("test/atlas.png")),
                texture_get_height(data_path("test/atlas.png")));
}

void sprite_init()
{
    /* initialize pool */
    pool = entitypool_new(Sprite);

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
    glBufferData(GL_ARRAY_BUFFER, entitypool_size(pool) * sizeof(Sprite),
                 entitypool_begin(pool), GL_STREAM_DRAW);
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

    /* deinit pool */
    entitypool_free(pool);
}

void sprite_update_all()
{
    Sprite *sprite, *end;

    for (sprite = entitypool_begin(pool);
         sprite != entitypool_end(pool); )
        if (entity_destroyed(sprite->pool_elem.ent))
            sprite_remove(sprite->pool_elem.ent);
        else
            ++sprite;

    for (sprite = entitypool_begin(pool), end = entitypool_end(pool);
         sprite != end; ++sprite)
        sprite->transform = transform_get_world_matrix(sprite->pool_elem.ent);
}

void sprite_draw_all()
{
    glUseProgram(program);
    glUniformMatrix3fv(glGetUniformLocation(program, "inverse_view_matrix"),
                       1, GL_FALSE,
                       (const GLfloat *) camera_get_inverse_view_matrix_ptr());

    glActiveTexture(GL_TEXTURE0);
    texture_bind(data_path("test/atlas.png"));

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, sprite_buf_object);
    glBufferData(GL_ARRAY_BUFFER, entitypool_size(pool) * sizeof(Sprite),
                 entitypool_begin(pool), GL_STREAM_DRAW);
    glDrawArrays(GL_POINTS, 0, entitypool_size(pool));
}

void sprite_save_all(Serializer *s)
{
    unsigned int n;
    Sprite *sprite, *end;

    n = entitypool_size(pool);
    uint_save(&n, s);

    for (sprite = entitypool_begin(pool), end = entitypool_end(pool);
         sprite != end; ++sprite)
    {
        entitypool_elem_save(pool, &sprite, s);
        mat3_save(&sprite->transform, s);
        vec2_save(&sprite->cell, s);
        vec2_save(&sprite->size, s);
    }
}
void sprite_load_all(Deserializer *s)
{
    unsigned int n;
    Sprite *sprite;

    entitypool_clear(pool);

    uint_load(&n, s);

    while (n--)
    {
        entitypool_elem_load(pool, &sprite, s);
        mat3_load(&sprite->transform, s);
        vec2_load(&sprite->cell, s);
        vec2_load(&sprite->size, s);
    }
}

