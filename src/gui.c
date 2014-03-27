#include "gui.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
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

/* --- common -------------------------------------------------------------- */

/*
 * general functionality/data common to all GUI systems
 */

typedef struct Gui Gui;
struct Gui
{
    EntityPoolElem pool_elem;

    Color color;

    BBox bbox; /* in entity space */
    GuiAlign halign;
    GuiAlign valign;
    Vec2 padding;
};

static EntityPool *gui_pool;

Entity gui_get_root()
{
    return gui_root;
}

void gui_add(Entity ent)
{
    Gui *gui;

    if (entitypool_get(gui_pool, ent))
        return; /* already has gui */

    transform_add(ent);

    gui = entitypool_add(gui_pool, ent);
    gui->color = color(0.5, 0.5, 0.5, 1.0);
    gui->bbox = bbox(vec2_zero, vec2(32, 32));
    gui->halign = GA_NONE;
    gui->valign = GA_NONE;
    gui->padding = vec2(6, 6);
}

void gui_remove(Entity ent)
{
    entitypool_remove(gui_pool, ent);
}

void gui_set_color(Entity ent, Color color)
{
    Gui *gui = entitypool_get(gui_pool, ent);
    assert(gui);
    gui->color = color;
}
Color gui_get_color(Entity ent)
{
    Gui *gui = entitypool_get(gui_pool, ent);
    assert(gui);
    return gui->color;
}

void gui_set_halign(Entity ent, GuiAlign align)
{
    Gui *gui = entitypool_get(gui_pool, ent);
    assert(gui);
    gui->halign = align;
}
GuiAlign gui_get_halign(Entity ent)
{
    Gui *gui = entitypool_get(gui_pool, ent);
    assert(gui);
    return gui->halign;
}
void gui_set_valign(Entity ent, GuiAlign align)
{
    Gui *gui = entitypool_get(gui_pool, ent);
    assert(gui);
    gui->valign = align;
}
GuiAlign gui_get_valign(Entity ent)
{
    Gui *gui = entitypool_get(gui_pool, ent);
    assert(gui);
    return gui->valign;
}
void gui_set_padding(Entity ent, Vec2 padding)
{
    Gui *gui = entitypool_get(gui_pool, ent);
    assert(gui);
    gui->padding = padding;
}
Vec2 gui_get_padding(Entity ent)
{
    Gui *gui = entitypool_get(gui_pool, ent);
    assert(gui);
    return gui->padding;
}

static void _common_init()
{
    gui_pool = entitypool_new(Gui);
}
static void _common_deinit()
{
    entitypool_free(gui_pool);
}

static void _common_update_align()
{
    Gui *gui, *pgui;
    BBox b, pb;
    Vec2 pos;
    Entity ent;
    Scalar mid, pmid;

    entitypool_foreach(gui, gui_pool)
    {
        if (gui->halign == GA_NONE && gui->valign == GA_NONE)
            continue;

        ent = gui->pool_elem.ent;

        /* get parent-space bounding box and position */
        b = bbox_transform(transform_get_matrix(ent), gui->bbox);
        pos = transform_get_position(ent);

        /* get parent gui and its bounding box */
        pgui = entitypool_get(gui_pool, transform_get_parent(ent));
        if (!pgui)
            continue;
        pb = pgui->bbox;

        /* macro to avoid repetition -- 'z' is Vec2 axis member (x or y) */
#define axis_align(align, z)                                            \
        switch (align)                                                  \
        {                                                               \
            case GA_MIN:                                                \
                pos.z = pb.min.z + gui->padding.z + pos.z - b.min.z;    \
                break;                                                  \
            case GA_MAX:                                                \
                pos.z = pb.max.z - gui->padding.z - (b.max.z - pos.z);  \
                break;                                                  \
            case GA_MID:                                                \
                mid = 0.5 * (b.min.z + b.max.z);                        \
                pmid = 0.5 * (pb.min.z + pb.max.z);                     \
                pos.z = pmid - (mid - pos.z);                           \
                break;                                                  \
            case GA_NONE:                                               \
                break;                                                  \
        }                                                               \

        axis_align(gui->halign, x);
        axis_align(gui->valign, y);
        transform_set_position(ent, pos);
    }
}

static void _common_update_all()
{
    Gui *gui;
    Entity ent;

    entitypool_remove_destroyed(gui_pool, gui_remove);

    /* attach root GUI entities to gui_root */
    entitypool_foreach(gui, gui_pool)
    {
        ent = gui->pool_elem.ent;
        if (!entity_eq(ent, gui_root)
            && entity_eq(transform_get_parent(ent), entity_nil))
            transform_set_parent(ent, gui_root);
    }

    /* update edit bboxes */
    if (edit_get_enabled())
        entitypool_foreach(gui, gui_pool)
            edit_bboxes_update(gui->pool_elem.ent, gui->bbox);
}

static void _common_save_all(Serializer *s)
{
    Gui *gui;

    entitypool_foreach(gui, gui_pool)
    {
        if (!entity_get_save_filter(gui->pool_elem.ent))
            continue;
        loop_continue_save(s);

        entitypool_elem_save(gui_pool, &gui, s);
        color_save(&gui->color, s);
        enum_save(&gui->halign, s);
        enum_save(&gui->valign, s);
        vec2_save(&gui->padding, s);
    }
    loop_end_save(s);
}
static void _common_load_all(Deserializer *s)
{
    Gui *gui;

    while (loop_continue_load(s))
    {
        entitypool_elem_load(gui_pool, &gui, s);
        color_load(&gui->color, s);
        enum_load(&gui->halign, s);
        enum_load(&gui->valign, s);
        vec2_load(&gui->padding, s);
    }
}

/* --- rect ---------------------------------------------------------------- */

typedef struct Rect Rect;
struct Rect
{
    EntityPoolElem pool_elem;

    Mat3 wmat;

    Vec2 size;
    Color color;
};

static EntityPool *rect_pool;

void gui_rect_add(Entity ent)
{
    Rect *rect;

    if (entitypool_get(rect_pool, ent))
        return;

    gui_add(ent);

    rect = entitypool_add(rect_pool, ent);
    rect->size = vec2(64, 64);
}
void gui_rect_remove(Entity ent)
{
    entitypool_remove(rect_pool, ent);
}

void gui_rect_set_size(Entity ent, Vec2 size)
{
    Rect *rect = entitypool_get(rect_pool, ent);
    assert(rect);
    rect->size = size;
}
Vec2 gui_rect_get_size(Entity ent)
{
    Rect *rect = entitypool_get(rect_pool, ent);
    assert(rect);
    return rect->size;
}

static GLuint rect_program;
static GLuint rect_vao;
static GLuint rect_vbo;

static void _rect_init()
{
    /* init pool */
    rect_pool = entitypool_new(Rect);

    /* create shader program, load texture, bind parameters */
    rect_program = gfx_create_program(data_path("rect.vert"),
                                      data_path("rect.geom"),
                                      data_path("rect.frag"));
    glUseProgram(rect_program);

    /* make vao, vbo, bind attributes */
    glGenVertexArrays(1, &rect_vao);
    glBindVertexArray(rect_vao);
    glGenBuffers(1, &rect_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, rect_vbo);
    gfx_bind_vertex_attrib(rect_program, GL_FLOAT, 3, "wmat1", Rect, wmat.m[0]);
    gfx_bind_vertex_attrib(rect_program, GL_FLOAT, 3, "wmat2", Rect, wmat.m[1]);
    gfx_bind_vertex_attrib(rect_program, GL_FLOAT, 3, "wmat3", Rect, wmat.m[2]);
    gfx_bind_vertex_attrib(rect_program, GL_FLOAT, 2, "size", Rect, size);
    gfx_bind_vertex_attrib(rect_program, GL_FLOAT, 4, "color", Rect, color);
}
static void _rect_deinit()
{
    /* deinit gl stuff */
    glDeleteProgram(rect_program);
    glDeleteBuffers(1, &rect_vbo);
    glDeleteVertexArrays(1, &rect_vao);

    /* deinit pool */
    entitypool_free(rect_pool);
}

static void _rect_update_all()
{
    Rect *rect;
    Gui *gui;

    entitypool_remove_destroyed(rect_pool, gui_rect_remove);

    entitypool_foreach(rect, rect_pool)
    {
        gui = entitypool_get(gui_pool, rect->pool_elem.ent);
        assert(gui);

        /* world matrix */
        rect->wmat = transform_get_world_matrix(rect->pool_elem.ent);

        /* write gui bbox */
        gui->bbox = bbox_bound(vec2_zero, vec2(rect->size.x, -rect->size.y));

        /* read gui properties */
        rect->color = gui->color;
    }
}

static void _rect_draw_all()
{
    unsigned int nrects;

    /* bind shader program */
    glUseProgram(rect_program);
    glUniformMatrix3fv(glGetUniformLocation(rect_program,
                                            "inverse_view_matrix"),
                       1, GL_FALSE,
                       (const GLfloat *) camera_get_inverse_view_matrix_ptr());

    /* draw! */
    glBindVertexArray(rect_vao);
    glBindBuffer(GL_ARRAY_BUFFER, rect_vbo);
    nrects = entitypool_size(rect_pool);
    glBufferData(GL_ARRAY_BUFFER, nrects * sizeof(Rect),
                 entitypool_begin(rect_pool), GL_STREAM_DRAW);
    glDrawArrays(GL_POINTS, 0, nrects);
}

static void _rect_save_all(Serializer *s)
{
    Rect *rect;

    entitypool_foreach(rect, rect_pool)
    {
        if (!entity_get_save_filter(rect->pool_elem.ent))
            continue;
        loop_continue_save(s);

        entitypool_elem_save(rect_pool, &rect, s);
        mat3_save(&rect->wmat, s);
        vec2_save(&rect->size, s);
        color_save(&rect->color, s);
    }
    loop_end_save(s);
}
static void _rect_load_all(Deserializer *s)
{
    Rect *rect;

    while (loop_continue_load(s))
    {
        entitypool_elem_load(rect_pool, &rect, s);
        mat3_load(&rect->wmat, s);
        vec2_load(&rect->size, s);
        color_load(&rect->color, s);
    }
}

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

    char *str;
    Array *chars;  /* per-character info buffered to shader */
    Vec2 bounds;   /* max x, min y in size-less units */
};

static EntityPool *text_pool;

static void _text_set_str(Text *text, const char *str)
{
    char c;
    TextChar *tc;
    Vec2 pos;

    /* copy to struct */
    free(text->str);
    text->str = malloc(strlen(str) + 1);
    strcpy(text->str, str);

    /* create TextChar array and update bounds */
    pos = vec2(0, -1);
    text->bounds = vec2(1, -1);
    array_clear(text->chars);
    while (*str)
    {
        c = *str++;
        switch (c)
        {
            case '\n':
                /* next line */
                pos.x = 0;
                pos.y -= 1;
                continue;

            case ' ':
                /* just skip ahead */
                pos.x += 1;
                text->bounds.x = scalar_max(text->bounds.x, pos.x);
                continue;
        }

        /* compute position in font grid */
        tc = array_add(text->chars);
        tc->pos = pos;
        tc->cell = vec2(c % TEXT_GRID_W, TEXT_GRID_H - 1 - (c / TEXT_GRID_W));

        /* move ahead */
        pos.x += 1;
        text->bounds.x = scalar_max(text->bounds.x, pos.x);
    }

    text->bounds.y = pos.y;
}

void gui_text_add(Entity ent)
{
    Text *text;

    if (entitypool_get(text_pool, ent))
        return; /* already has text */

    gui_add(ent);

    text = entitypool_add(text_pool, ent);
    text->chars = array_new(TextChar);
    text->str = NULL; /* _text_set_str(...) calls free(text->str) */
    _text_set_str(text, "");
}
void gui_text_remove(Entity ent)
{
    Text *text = entitypool_get(text_pool, ent);
    if (text)
    {
        free(text->str);
        array_free(text->chars);
    }
    entitypool_remove(text_pool, ent);
}

void gui_text_set_str(Entity ent, const char *str)
{
    Text *text = entitypool_get(text_pool, ent);
    assert(text);
    _text_set_str(text, str);
}
const char *gui_text_get_str(Entity ent)
{
    Text *text = entitypool_get(text_pool, ent);
    assert(text);
    return text->str;
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
    Text *text;
    
    /* deinit gl stuff */
    glDeleteProgram(text_program);
    glDeleteBuffers(1, &text_vbo);
    glDeleteVertexArrays(1, &text_vao);

    /* deinit pool */
    entitypool_foreach(text, text_pool)
    {
        free(text->str);
        array_free(text->chars);
    }
    entitypool_free(text_pool);
}

static void _text_update_all()
{
    Text *text;
    Gui *gui;
    static Vec2 size = { TEXT_FONT_W, TEXT_FONT_H };

    entitypool_remove_destroyed(text_pool, gui_text_remove);

    entitypool_foreach(text, text_pool)
    {
        /* gui bbox */
        gui = entitypool_get(gui_pool, text->pool_elem.ent);
        assert(gui);
        gui->bbox = bbox_bound(vec2_zero, vec2_mul(size, text->bounds));
    }
}

static void _text_draw_all()
{
    Vec2 hwin;
    Text *text;
    Gui *gui;
    Mat3 wmat;
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
        wmat = transform_get_world_matrix(text->pool_elem.ent);
        glUniformMatrix3fv(glGetUniformLocation(text_program, "wmat"),
                           1, GL_FALSE, (const GLfloat *) &wmat);

        gui = entitypool_get(gui_pool, text->pool_elem.ent);
        assert(gui);
        glUniform4fv(glGetUniformLocation(text_program, "base_color"), 1,
                     (const GLfloat *) &gui->color);

        glBindVertexArray(text_vao);
        glBindBuffer(GL_ARRAY_BUFFER, text_vbo);
        nchars = array_length(text->chars);
        glBufferData(GL_ARRAY_BUFFER, nchars * sizeof(TextChar),
                     array_begin(text->chars), GL_STREAM_DRAW);
        glDrawArrays(GL_POINTS, 0, nchars);
    }
}

static void _text_save_all(Serializer *s)
{
    Text *text;
    TextChar *tc;
    unsigned int nchars;

    entitypool_foreach(text, text_pool)
    {
        if (!entity_get_save_filter(text->pool_elem.ent))
            continue;
        loop_continue_save(s);

        entitypool_elem_save(text_pool, &text, s);

        string_save((const char **) &text->str, s);

        nchars = array_length(text->chars);
        uint_save(&nchars, s);
        array_foreach(tc, text->chars)
        {
            vec2_save(&tc->pos, s);
            vec2_save(&tc->cell, s);
        }

        vec2_save(&text->bounds, s);
    }
    loop_end_save(s);
}
static void _text_load_all(Deserializer *s)
{
    Text *text;
    TextChar *tc;
    unsigned int nchars;

    while(loop_continue_load(s))
    {
        entitypool_elem_load(text_pool, &text, s);

        string_load(&text->str, s);

        uint_load(&nchars, s);
        text->chars = array_new(TextChar);
        array_reset(text->chars, nchars);
        array_foreach(tc, text->chars)
        {
            vec2_load(&tc->pos, s);
            vec2_load(&tc->cell, s);
        }

        vec2_load(&text->bounds, s);
    }
}

/* ------------------------------------------------------------------------- */

static void _create_root()
{
    gui_root = entity_create();
    transform_add(gui_root);
    transform_set_position(gui_root, vec2(-1, 1)); /* origin at top-left */
    gui_add(gui_root);
}

void gui_init()
{
    _common_init();
    _rect_init();
    _text_init();
    _create_root();
}
void gui_deinit()
{
    _text_deinit();
    _rect_deinit();
    _common_deinit();
}

static void _update_root()
{
    Gui *gui;
    Vec2 win_size;

    win_size = game_get_window_size();

    edit_set_editable(gui_root, false);

    /* child of camera so GUI stays on screen */
    transform_set_parent(gui_root, camera_get());

    /* use pixel coordinates */
    transform_set_scale(gui_root, scalar_vec2_div(2, win_size));
    gui = entitypool_get(gui_pool, gui_root);
    gui->bbox = bbox_bound(vec2_zero, vec2(win_size.x, -win_size.y));
}

void gui_update_all()
{
    _update_root();
    _common_update_align();
    _rect_update_all();
    _text_update_all();
    _common_update_all();
}

void gui_draw_all()
{
    _rect_draw_all();
    _text_draw_all();
}

void gui_save_all(Serializer *s)
{
    _common_save_all(s);
    _rect_save_all(s);
    _text_save_all(s);
}
void gui_load_all(Deserializer *s)
{
    _common_load_all(s);
    _rect_load_all(s);
    _text_load_all(s);
}
