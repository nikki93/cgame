#include "sprite.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <stb_image.h>

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

static Sprite *sprite_buf = NULL;        /* all sprites tightly packed */
static unsigned int sprite_buf_capacity;
static unsigned int num_sprites = 0;

static EntityMap *emap;                  /* map of pointers into sprite_buf */

/* ------------------------------------------------------------------------- */

void sprite_add(Entity ent)
{
    Sprite *sprite;

    /* already has sprite? */
    if (entitymap_get(emap, ent))
        return;

    /* maybe realloc buffer */
    if (++num_sprites > sprite_buf_capacity)
    {
        sprite_buf_capacity <<= 1;
        sprite_buf = realloc(sprite_buf, sprite_buf_capacity * sizeof(Sprite));
    }

    /* claim new spot at end */
    sprite = &sprite_buf[num_sprites - 1];
    sprite->entity = ent;
    sprite->cell.x = 32.0f; sprite->cell.y = 32.0f;
    sprite->size.x = 32.0f; sprite->size.y = 32.0f;
    entitymap_set(emap, ent, sprite);
}
void sprite_remove(Entity ent)
{
    Sprite *sprite = entitymap_get(emap, ent);
    entitymap_set(emap, ent, NULL);

    /* replace with last sprite */
    if (sprite != &sprite_buf[num_sprites - 1] && num_sprites > 1)
    {
        *sprite = sprite_buf[num_sprites - 1];
        entitymap_set(emap, sprite->entity, sprite);
    }

    /* maybe realloc buffer */
    if (--num_sprites << 2 < sprite_buf_capacity)
    {
        sprite_buf_capacity >>= 1;
        sprite_buf = realloc(sprite_buf, sprite_buf_capacity * sizeof(Sprite));
    }
}

void sprite_set_cell(Entity ent, Vec2 cell)
{
    Sprite *sprite = entitymap_get(emap, ent);
    assert(sprite);

    sprite->cell = cell;
}
void sprite_set_size(Entity ent, Vec2 size)
{
    Sprite *sprite = entitymap_get(emap, ent);
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
    unsigned int i;

    /* initialize buffer */
    sprite_buf_capacity = 2;
    sprite_buf = malloc(sprite_buf_capacity * sizeof(Sprite));

    /* initialize map */
    emap = entitymap_new(NULL);

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
    glBufferData(GL_ARRAY_BUFFER, num_sprites * sizeof(Sprite), sprite_buf,
            GL_STREAM_DRAW);
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

    /* free entity map */
    entitymap_free(emap);

    /* free buffer */
    free(sprite_buf);
}

void sprite_update_all()
{
    unsigned int i;

    for (i = 0; i < num_sprites; ++i)
        sprite_buf[i].transform =
            transform_get_world_matrix(sprite_buf[i].entity);
}

void sprite_draw_all()
{
    glBindVertexArray(vao);

    glUniformMatrix3fv(glGetUniformLocation(program, "inverse_view_matrix"),
            1, GL_FALSE,
            (const GLfloat *) camera_get_inverse_view_matrix_ptr());

    glBindBuffer(GL_ARRAY_BUFFER, sprite_buf_object);
    glBufferData(GL_ARRAY_BUFFER, num_sprites * sizeof(Sprite), sprite_buf,
            GL_STREAM_DRAW);
    glDrawArrays(GL_POINTS, 0, num_sprites);
}

void sprite_save_all(FILE *file)
{
    unsigned int i;

    uint_save(&num_sprites, file);

    for (i = 0; i < num_sprites; ++i)
    {
        entity_save(&sprite_buf[i].entity, file);

        mat3_save(&sprite_buf[i].transform, file);

        vec2_save(&sprite_buf[i].cell, file);
        vec2_save(&sprite_buf[i].size, file);
    }
}
void sprite_load_all(FILE *file)
{
    unsigned int i;

    uint_load(&num_sprites, file);

    /* keep it simple -- make capacity just fit */
    sprite_buf_capacity = num_sprites << 1;
    sprite_buf = realloc(sprite_buf, sprite_buf_capacity * sizeof(Sprite));
    for (i = 0; i < num_sprites; ++i)
    {
        entity_load(&sprite_buf[i].entity, file);

        mat3_load(&sprite_buf[i].transform, file);

        vec2_load(&sprite_buf[i].cell, file);
        vec2_load(&sprite_buf[i].size, file);
    }

    /* restore map */
    entitymap_clear(emap);
    for (i = 0; i < num_sprites; ++i)
        entitymap_set(emap, sprite_buf[i].entity, &sprite_buf[i]);
}

