#include "text.h"

#include <stdlib.h>
#include <assert.h>
#include <GL/glew.h>

#include "gfx.h"
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
    bool visible;
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

/*
 * do a simple linear search for now, will use better data structure later
 * if needed
 */
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
    info->visible = true;
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
    TextInfo *info = _find(text);
    assert(info);
    info->pos = pos;
}

void text_set_str(Text text, const char *str)
{
    TextInfo *info = _find(text);
    assert(info);
    _gen_chars(info->chars, str);
}

void text_set_visible(Text text, bool visible)
{
    TextInfo *info = _find(text);
    assert(info);
    info->visible = visible;
}
bool text_get_visible(Text text)
{
    TextInfo *info = _find(text);
    assert(info);
    return info->visible;
}

static GLuint program;
static GLuint vao;
static GLuint vbo;

static void _init_gl()
{
    /* create shader program, load texture, bind parameters */
    program = gfx_create_program(data_path("text.vert"),
                                 data_path("text.geom"),
                                 data_path("text.frag"));
    glUseProgram(program);
    texture_load(data_path("font1.png"));
    glUniform1i(glGetUniformLocation(program, "tex0"), 0);
    glUniform2f(glGetUniformLocation(program, "inv_grid_size"),
                1.0 / GRID_W, 1.0 / GRID_H);

    /* make vao, vbo, bind attributes */
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    gfx_bind_vertex_attrib(program, GL_FLOAT, 2, "pos", TextChar, pos);
    gfx_bind_vertex_attrib(program, GL_FLOAT, 2, "cell", TextChar, cell);
}
static void _deinit_gl()
{
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo);
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

    /* bind shader program */
    glUseProgram(program);
    glUniform2f(glGetUniformLocation(program, "size"),
                FONT_W / hwin.x, FONT_H / hwin.y);

    /* bind texture */
    glActiveTexture(GL_TEXTURE0);
    texture_bind(data_path("font1.png"));

    /* draw! */
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    for (info = array_begin(infos), end = array_end(infos);
         info != end; ++info)
    {
        if (!info->visible)
            continue;

        glUniform2f(glGetUniformLocation(program, "base_pos"),
                    info->pos.x / hwin.x - 1,
                    info->pos.y / hwin.y + 1);

        nchars = array_length(info->chars);
        glBufferData(GL_ARRAY_BUFFER, nchars * sizeof(TextChar),
                     array_begin(info->chars), GL_STREAM_DRAW);
        glDrawArrays(GL_POINTS, 0, nchars);
    }
}

