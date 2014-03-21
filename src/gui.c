#include "gui.h"

#include <assert.h>
#include <GL/glew.h>

#include "entitypool.h"
#include "mat3.h"
#include "array.h"
#include "transform.h"
#include "gfx.h"
#include "texture.h"
#include "dirs.h"
#include "game.h"
#include "camera.h"
#include "edit.h"

static Entity gui_root; /* all gui should be descendants of this to move with screen */

/* --- text ---------------------------------------------------------------- */

#define TEXT_GRID_W 16
#define TEXT_GRID_H 16

#define TEXT_FONT_W 10
#define TEXT_FONT_H 12

/* info to send to shader program for each character */
typedef struct TextChar TextChar;
struct TextChar
{
    Vec2 pos;   /* position in space of text entity in size-less units */
    Vec2 cell;  /* cell in font image */
};

/* info per text entity */
typedef struct Text Text;
struct Text
{
    EntityPoolElem pool_elem;

    Color color;

    Mat3 wmat;
    Array *chars;  /* per-character info buffered to shader */

    Vec2 bounds; /* max x, min y of text region in size-less units */
};

static EntityPool *text_pool;

void gui_text_add(Entity ent)
{
    Text *text;

    if (entitypool_get(text_pool, ent))
        return; /* already has text */

    transform_add(ent);
    transform_set_parent(ent, gui_root);

    text = entitypool_add(text_pool, ent);
    text->chars = array_new(TextChar);
    text->color = color_black;
}
void gui_text_remove(Entity ent)
{
    Text *text = entitypool_get(text_pool, ent);
    if (text)
        array_free(text->chars);
    entitypool_remove(text_pool, ent);
}

/* update char array and bounds */
static void _text_update_str(Text *text, const char *str)
{
    char c;
    TextChar *tc;
    Vec2 pos;

    pos = vec2(0, -1);

    text->bounds = vec2(1, -1);
    array_clear(text->chars);
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
                text->bounds.x = scalar_max(text->bounds.x, pos.x);
                continue;
        }

        tc = array_add(text->chars);
        tc->pos = pos;
        tc->cell = vec2(c % TEXT_GRID_W, TEXT_GRID_H - 1 - (c / TEXT_GRID_W));

        pos.x += 1;
        text->bounds.x = scalar_max(text->bounds.x, pos.x);
    }

    text->bounds.y = pos.y;
}

void gui_text_set_str(Entity ent, const char *str)
{
    Text *text = entitypool_get(text_pool, ent);
    assert(text);
    _text_update_str(text, str);
}

void gui_text_set_color(Entity ent, Color color)
{
    Text *text = entitypool_get(text_pool, ent);
    assert(text);
    text->color = color;
}
Color gui_text_get_color(Entity ent)
{
    Text *text = entitypool_get(text_pool, ent);
    assert(text);
    return text->color;
}

static GLuint text_program;
static GLuint text_vao;
static GLuint text_vbo;

static void _text_init()
{
    /* init pool */
    text_pool = entitypool_new(Text);

    /* create shader program, load texture, bind parameters */
    text_program = gfx_create_program(data_path("text.vert"),
                                 data_path("text.geom"),
                                 data_path("text.frag"));
    glUseProgram(text_program);
    texture_load(data_path("font1.png"));
    glUniform1i(glGetUniformLocation(text_program, "tex0"), 0);
    glUniform2f(glGetUniformLocation(text_program, "inv_grid_size"),
                1.0 / TEXT_GRID_W, 1.0 / TEXT_GRID_H);
    glUniform2f(glGetUniformLocation(text_program, "size"),
                TEXT_FONT_W, TEXT_FONT_H);

    /* make vao, vbo, bind attributes */
    glGenVertexArrays(1, &text_vao);
    glBindVertexArray(text_vao);
    glGenBuffers(1, &text_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, text_vbo);
    gfx_bind_vertex_attrib(text_program, GL_FLOAT, 2, "pos", TextChar, pos);
    gfx_bind_vertex_attrib(text_program, GL_FLOAT, 2, "cell", TextChar, cell);
}
static void _text_deinit()
{
    /* deinit gl stuff */
    glDeleteProgram(text_program);
    glDeleteBuffers(1, &text_vbo);
    glDeleteVertexArrays(1, &text_vao);

    /* deinit pool */
    entitypool_free(text_pool);
}

static void _text_update_all()
{
    Text *text;
    BBox bbox;
    static Vec2 size = { TEXT_FONT_W, TEXT_FONT_H };

    entitypool_remove_destroyed(text_pool, gui_text_remove);

    entitypool_foreach(text, text_pool)
        text->wmat = transform_get_world_matrix(text->pool_elem.ent);

    if (edit_get_enabled())
        entitypool_foreach(text, text_pool)
        {
            bbox = bbox_bound(vec2_zero, vec2_mul(size, text->bounds));
            edit_bboxes_update(text->pool_elem.ent, bbox);
        }
}

static void _text_draw_all()
{
    Vec2 hwin;
    Text *text;
    unsigned int nchars;

    hwin = vec2_scalar_mul(game_get_window_size(), 0.5);

    /* bind shader program */
    glUseProgram(text_program);
    glUniformMatrix3fv(glGetUniformLocation(text_program,
                                            "inverse_view_matrix"),
                       1, GL_FALSE,
                       (const GLfloat *) camera_get_inverse_view_matrix_ptr());


    /* bind texture */
    glActiveTexture(GL_TEXTURE0);
    texture_bind(data_path("font1.png"));

    /* draw! */
    entitypool_foreach(text, text_pool)
    {
        glUniformMatrix3fv(glGetUniformLocation(text_program, "wmat"),
                           1, GL_FALSE, (const GLfloat *) &text->wmat);
        glUniform4fv(glGetUniformLocation(text_program, "base_color"), 1,
                     (const GLfloat *) &text->color);

        glBindVertexArray(text_vao);
        glBindBuffer(GL_ARRAY_BUFFER, text_vbo);
        nchars = array_length(text->chars);
        glBufferData(GL_ARRAY_BUFFER, nchars * sizeof(TextChar),
                     array_begin(text->chars), GL_STREAM_DRAW);
        glDrawArrays(GL_POINTS, 0, nchars);
    }
}

/* ------------------------------------------------------------------------- */

static void _create_root()
{
    gui_root = entity_create();
    entity_set_persistent(gui_root, true);
    transform_add(gui_root);
    transform_set_position(gui_root, vec2(-1, 1)); /* origin at top-left */
}

void gui_init()
{
    _create_root();
    _text_init();
}
void gui_deinit()
{
    _text_deinit();
}

static void _update_gui_root()
{
    /* child of camera so GUI stays on screen */
    transform_set_parent(gui_root, camera_get());

    /* use pixel coordinates */
    transform_set_scale(gui_root, scalar_vec2_div(2, game_get_window_size()));
}

void gui_update_all()
{
    _update_gui_root();
    _text_update_all();
}

void gui_draw_all()
{
    _text_draw_all();
}
