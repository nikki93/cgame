#include "sprite.h"

#include <assert.h>
#include <stdio.h>
#include <GL/glew.h>
#include <Freeimage.h>

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

static unsigned int num_sprites = 0;
static Sprite sprites[ENTITY_MAX];            /* all sprites tightly packed */
static Sprite *entity_sprite[ENTITY_MAX];     /* pointers into above */

/* ------------------------------------------------------------------------- */

void sprite_add(Entity ent)
{
    Sprite *sprite;

    if (entity_sprite[ent])
        return; /* already has a sprite */

    sprite = &sprites[num_sprites++];
    entity_sprite[ent] = sprite;

    sprite->entity = ent;
    sprite->cell.x = 32.0f; sprite->cell.y = 32.0f;
    sprite->size.x = 32.0f; sprite->size.y = 32.0f;
}
void sprite_remove(Entity ent)
{
    Sprite *old_sprite = entity_sprite[ent];
    entity_sprite[ent] = NULL;

    /* replace with last sprite */
    if (old_sprite != &sprites[num_sprites - 1] && num_sprites > 1)
    {
        *old_sprite = sprites[num_sprites - 1];
        entity_sprite[old_sprite->entity] = old_sprite;
    }

    --num_sprites;
}

void sprite_set_cell(Entity ent, Vec2 cell)
{
    Sprite *sprite = entity_sprite[ent];
    assert(sprite);

    sprite->cell = cell;
}
void sprite_set_size(Entity ent, Vec2 size)
{
    Sprite *sprite = entity_sprite[ent];
    assert(sprite);

    sprite->size = size;
}

/* ------------------------------------------------------------------------- */

static GLuint vertex_shader;
static GLuint geometry_shader;
static GLuint fragment_shader;
static GLuint program;

static GLuint vao;
static GLuint buffer_object;

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

void sprite_init()
{
    unsigned int i;

    for (i = 0; i < ENTITY_MAX; ++i)
        entity_sprite[i] = NULL;

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
    glGenBuffers(1, &buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_object);
    glBufferData(GL_ARRAY_BUFFER, num_sprites * sizeof(Sprite), sprites,
            GL_STREAM_DRAW);
    _bind_attributes();

    /* load and use atlas texture */
    glGenTextures(1, &atlas_tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlas_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    FIBITMAP *img = FreeImage_ConvertTo32Bits(FreeImage_Load(
                FreeImage_GetFileType(data_path("atlas.png"), 0),
                data_path("atlas.png"), 0));
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
            FreeImage_GetWidth(img), FreeImage_GetHeight(img),
            0, GL_BGRA, GL_UNSIGNED_BYTE, FreeImage_GetBits(img));
    FreeImage_Unload(img);
    glUniform1i(glGetUniformLocation(program, "tex0"), 0);
    glUniform2f(glGetUniformLocation(program, "atlas_size"),
            FreeImage_GetWidth(img), FreeImage_GetHeight(img));
}

void sprite_deinit()
{
    /* clean up OpenGL stuff */
    glDeleteProgram(program);
    glDeleteShader(fragment_shader);
    glDeleteShader(geometry_shader);
    glDeleteShader(vertex_shader);
    glDeleteBuffers(1, &buffer_object);
    glDeleteVertexArrays(1, &vao);
}

void sprite_check_messages_all()
{
    unsigned int i;
    Message *msg;
    Entity ent;

    for (i = 0; i < num_sprites; ++i)
    {
        ent = sprites[i].entity;

        for (msg = entity_get_first_message(ent); msg;
                msg = entity_get_next_message(msg))
            if (message_get_type(msg) == MSG_DESTROY)
            {
                sprite_remove(ent);
                break;
            }
    }
}

void sprite_update_all()
{
    unsigned int i;

    for (i = 0; i < num_sprites; ++i)
        sprites[i].transform = transform_get_world_matrix(sprites[i].entity);
}

void sprite_draw_all()
{
    glBindVertexArray(vao);

    glUniformMatrix3fv(glGetUniformLocation(program, "inverse_view_matrix"),
            1, GL_FALSE,
            (const GLfloat *) camera_get_inverse_view_matrix_ptr());

    glBindBuffer(GL_ARRAY_BUFFER, buffer_object);
    glBufferData(GL_ARRAY_BUFFER, num_sprites * sizeof(Sprite), sprites,
            GL_STREAM_DRAW);
    glDrawArrays(GL_POINTS, 0, num_sprites);
}

void sprite_save_all(FILE *file)
{
    unsigned int i;

    save_uint(&num_sprites, file);

    for (i = 0; i < num_sprites; ++i)
    {
        entity_save(&sprites[i].entity, file);

        mat3_save(&sprites[i].transform, file);

        vec2_save(&sprites[i].cell, file);
        vec2_save(&sprites[i].size, file);
    }
}

void sprite_load_all(FILE *file)
{
    unsigned int i;

    load_uint(&num_sprites, file);

    for (i = 0; i < num_sprites; ++i)
    {
        entity_load(&sprites[i].entity, file);

        mat3_load(&sprites[i].transform, file);

        vec2_load(&sprites[i].cell, file);
        vec2_load(&sprites[i].size, file);
    }

    /* restore entity <-> sprite connections */
    for (i = 0; i < ENTITY_MAX; ++i)
        entity_sprite[i] = NULL;
    for (i = 0; i < num_sprites; ++i)
        entity_sprite[sprites[i].entity] = &sprites[i];
}

