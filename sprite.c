#include "sprite.h"

#include <stdio.h>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <Freeimage.h>

static unsigned int num_sprites = 0;

typedef struct Sprite Sprite;
struct Sprite
{
    Vec2 position;
    Vec2 cell;
    Vec2 size;
};

static Sprite sprites[ENTITY_MAX];

/* ------------------------------------------------------------------------- */

void sprite_add(Entity ent)
{
    if (ent + 1 > num_sprites)
        num_sprites = ent + 1;
}

void sprite_set_cell(Entity ent, Vec2 cell)
{
    sprites[ent].cell = cell;
}
void sprite_set_size(Entity ent, Vec2 size)
{
    sprites[ent].size = size;
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

    /* log */
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    printf(status ? "successful\n" : "unsuccessful\n");
    glGetShaderInfoLog(shader, 512, NULL, log);
    printf("%s\n", log);
}

/* get pointer offset of 'field' in struct 'type' */
#define poffsetof(type, field) \
    ((void *) (&((type *) 0)->field))

static void _bind_attributes(GLuint position, GLuint cell, GLuint size)
{
    glVertexAttribPointer(position, 2, GL_FLOAT, GL_FALSE,
            sizeof(Sprite), poffsetof(Sprite, position));
    glEnableVertexAttribArray(position);
    /* glVertexAttribDivisor(position, divisor); */

    glVertexAttribPointer(cell, 2, GL_FLOAT, GL_FALSE,
            sizeof(Sprite), poffsetof(Sprite, cell));
    glEnableVertexAttribArray(cell);
    /* glVertexAttribDivisor(cell, divisor); */

    glVertexAttribPointer(size, 2, GL_FLOAT, GL_FALSE,
            sizeof(Sprite), poffsetof(Sprite, size));
    glEnableVertexAttribArray(size);
    /* glVertexAttribDivisor(size, divisor); */
}

void sprite_init()
{
    /* compile shaders */
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    _compile_shader(vertex_shader, "sprite.vert");
    geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
    _compile_shader(geometry_shader, "sprite.geom");
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    _compile_shader(fragment_shader, "sprite.frag");

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
    _bind_attributes(glGetAttribLocation(program, "position"),
            glGetAttribLocation(program, "cell"),
            glGetAttribLocation(program, "size"));

    /* load and use atlas texture */
    glGenTextures(1, &atlas_tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlas_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    FIBITMAP *img = FreeImage_ConvertTo32Bits(FreeImage_Load(
                FreeImage_GetFileType("atlas.png", 0), "atlas.png", 0));
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

void sprite_update_all()
{
    for (unsigned int i = 0; i < num_sprites; ++i)
        sprites[i].position = transform_get_origin(i);
}

void sprite_draw_all()
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_object);
    glBufferData(GL_ARRAY_BUFFER, num_sprites * sizeof(Sprite), sprites,
            GL_STREAM_DRAW);
    glDrawArrays(GL_POINTS, 0, num_sprites);
}

