#include "text.h"

#include <stdlib.h>
#include <GL/glew.h>

#include "texture.h"
#include "dirs.h"
#include "array.h"
#include "game.h"

#define GRID_W 16
#define GRID_H 16

#define FONT_W 10
#define FONT_H 12

typedef struct TextInfo TextInfo;
struct TextInfo
{
    Text id;
    Vec2 pos; /* position of text in pixels */
    Array *chars;
};

typedef struct TextChar TextChar;
struct TextChar
{
    /* all x, y in unit multiples -- will be scaled in geometry shader */
    Vec2 pos;
    Vec2 cell;
};

static Array *infos;

/* generate TextChar objects from a string */
static void _gen_chars(Array *chars, const char *str)
{
    char c;
    TextChar *tc;
    Vec2 pos;
    
    pos = vec2(0, -1);
    array_reset(chars, 0);
    while (*str)
    {
        c = *str++;
        switch (c)
        {
            case '\n':
                pos.x = 0;
                pos.y -= 1;
                continue;

            case ' ':
                pos.x += 1;
                continue;
        }

        tc = array_add(chars);
        tc->pos = pos;
        tc->cell = vec2(c % GRID_W, GRID_H - 1 - (c / GRID_W));

        pos.x += 1;
    }
}

static TextInfo *_find(Text text)
{
    TextInfo *info, *end;

    for (info = array_begin(infos), end = array_end(infos);
         info != end; ++info)
        if (info->id == text)
            return info;
    return NULL;
}

Text text_add(Vec2 pos, const char *str)
{
    static Text curr = 0;

    TextInfo *info = array_add(infos);
    info->id = curr++;
    info->pos = pos;
    info->chars = array_new(TextChar);
    _gen_chars(info->chars, str);

    return info->id;
}
void text_remove(Text text)
{
    TextInfo *info = _find(text);
    if (!info)
        return;

    array_free(info->chars);
    array_quick_remove(infos, info - (TextInfo *) array_begin(infos));
}

void text_set_pos(Text text, Vec2 pos)
{
    /* TODO: implement this */
}
void text_set_str(Text text, const char *str)
{
    /* TODO: implement this */
}

static GLuint vertex_shader;
static GLuint geometry_shader;
static GLuint fragment_shader;
static GLuint program;

static GLuint vao;
static GLuint buf_obj;

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

    printf("text: compiling shader '%s' ...", filename);

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
    GLuint pos, cell;

    pos = glGetAttribLocation(program, "pos");
    glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE,
                          sizeof(TextChar), poffsetof(TextChar, pos));
    glEnableVertexAttribArray(pos);

    cell = glGetAttribLocation(program, "cell");
    glVertexAttribPointer(cell, 2, GL_FLOAT, GL_FALSE,
                          sizeof(TextChar), poffsetof(TextChar, cell));
    glEnableVertexAttribArray(cell);
}

static void _init_gl()
{
    /* compile shaders */
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    _compile_shader(vertex_shader, data_path("text.vert"));
    geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
    _compile_shader(geometry_shader, data_path("text.geom"));
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    _compile_shader(fragment_shader, data_path("text.frag"));

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
    glGenBuffers(1, &buf_obj);
    glBindBuffer(GL_ARRAY_BUFFER, buf_obj);
    glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STREAM_DRAW);
    _bind_attributes();

    /* load font texture */
    texture_load(data_path("font1.png"));
    glUniform1i(glGetUniformLocation(program, "tex0"), 0);
    glUniform2f(glGetUniformLocation(program, "inv_grid_size"),
                1.0 / GRID_W, 1.0 / GRID_H);
}
static void _deinit_gl()
{
    glDeleteProgram(program);
    glDeleteShader(fragment_shader);
    glDeleteShader(geometry_shader);
    glDeleteShader(vertex_shader);
    glDeleteBuffers(1, &buf_obj);
    glDeleteVertexArrays(1, &vao);
}

void text_init()
{
    infos = array_new(TextInfo);
    _init_gl();
}
void text_deinit()
{
    TextInfo *info, *end;

    _deinit_gl();

    for (info = array_begin(infos), end = array_end(infos);
         info != end; ++info)
        array_free(info->chars);
    array_free(infos);
}

void text_draw_all()
{
    Vec2 hwin;
    TextInfo *info, *end;
    unsigned int nchars;

    hwin = vec2_scalar_mul(game_get_window_size(), 0.5);

    glUseProgram(program);
    glUniform2f(glGetUniformLocation(program, "size"),
                FONT_W / hwin.x, FONT_H / hwin.y);

    glActiveTexture(GL_TEXTURE0);
    texture_bind(data_path("font1.png"));

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, buf_obj);
    for (info = array_begin(infos), end = array_end(infos);
         info != end; ++info)
    {
        glUniform2f(glGetUniformLocation(program, "base_pos"),
                    info->pos.x / hwin.x - 1,
                    info->pos.y / hwin.y + 1);

        nchars = array_length(info->chars);
        glBufferData(GL_ARRAY_BUFFER, nchars * sizeof(TextChar),
                     array_begin(info->chars), GL_STREAM_DRAW);
        glDrawArrays(GL_POINTS, 0, nchars);
    }
}

