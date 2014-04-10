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
#include "entitymap.h"
#include "timing.h"

static Entity gui_root; /* all gui should be descendants of this to move
                           with screen */

static Entity focused; /* currently focused entity, or entity_nil if none */

/* --- common -------------------------------------------------------------- */

/*
 * general functionality/data common to all GUI systems
 */

typedef struct Gui Gui;
struct Gui
{
    EntityPoolElem pool_elem;

    bool setvisible;      /* externally-set visibility */
    bool visible;         /* internal recursively computed visibility */
    bool updated_visible; /* for recursive visibility computation */
    bool focusable;       /* can be focused */

    Color color;

    BBox bbox; /* in entity space */
    GuiAlign halign;
    GuiAlign valign;
    Vec2 padding;
};

static EntityPool *gui_pool;

static EntityMap *focus_enter_map;
static EntityMap *focus_exit_map;
static EntityMap *mouse_down_map;
static EntityMap *mouse_up_map;

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
    gui->visible = true;
    gui->setvisible = true;
    gui->focusable = false;
    gui->color = color(0.5, 0.5, 0.5, 1.0);
    gui->bbox = bbox(vec2_zero, vec2(32, 32));
    gui->halign = GA_NONE;
    gui->valign = GA_NONE;
    gui->padding = vec2(5, 5);
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

void gui_set_visible(Entity ent, bool visible)
{
    Gui *gui = entitypool_get(gui_pool, ent);
    assert(gui);
    gui->setvisible = visible;
}
bool gui_get_visible(Entity ent)
{
    Gui *gui = entitypool_get(gui_pool, ent);
    assert(gui);
    return gui->visible;
}

void gui_set_focusable(Entity ent, bool focusable)
{
    Gui *gui = entitypool_get(gui_pool, ent);
    assert(gui);
    gui->focusable = focusable;
}
bool gui_get_focusable(Entity ent)
{
    Gui *gui = entitypool_get(gui_pool, ent);
    assert(gui);
    return gui->focusable;
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

void gui_set_focused_entity(Entity ent)
{
    if (entity_eq(focused, ent))
        return;

    if (entity_eq(ent, entity_nil))
        entitymap_set(focus_exit_map, ent, true);
    focused = ent;
    if (!entity_eq(focused, entity_nil))
        entitymap_set(focus_enter_map, focused, true);
}
Entity gui_get_focused_entity()
{
    return focused;
}
void gui_set_focus(Entity ent, bool focus)
{
    if (focus)
        gui_set_focused_entity(ent);
    else if (entity_eq(focused, ent))
        gui_set_focused_entity(entity_nil);
}
bool gui_get_focus(Entity ent)
{
    return entity_eq(focused, ent);
}
bool gui_has_focus()
{
    return !entity_eq(focused, entity_nil);
}

bool gui_event_focus_enter(Entity ent)
{
    return entitymap_get(focus_enter_map, ent);
}
bool gui_event_focus_exit(Entity ent)
{
    return entitymap_get(focus_exit_map, ent);
}
MouseCode gui_event_mouse_down(Entity ent)
{
    return entitymap_get(mouse_down_map, ent);
}
MouseCode gui_event_mouse_up(Entity ent)
{
    return entitymap_get(mouse_up_map, ent);
}

static void _common_init()
{
    gui_pool = entitypool_new(Gui);
    focus_enter_map = entitymap_new(false);
    focus_exit_map = entitymap_new(false);
    mouse_down_map = entitymap_new(MC_NONE);
    mouse_up_map = entitymap_new(MC_NONE);
}
static void _common_deinit()
{
    entitymap_free(mouse_up_map);
    entitymap_free(mouse_down_map);
    entitymap_free(focus_enter_map);
    entitymap_free(focus_exit_map);
    entitypool_free(gui_pool);
}

static void _common_update_destroyed()
{
    entitypool_remove_destroyed(gui_pool, gui_remove);
}

static void _common_update_visible_rec(Gui *gui)
{
    Gui *pgui;

    if (gui->updated_visible)
        return;

    /* false visibility takes priority */
    if (!gui->setvisible)
    {
        gui->visible = false;
        gui->updated_visible = true;
        return;
    }

    /* if has parent, inherit */
    pgui = entitypool_get(gui_pool, transform_get_parent(gui->pool_elem.ent));
    if (pgui)
    {
        _common_update_visible_rec(pgui);
        gui->visible = pgui->visible;
        gui->updated_visible = true;
        return;
    }

    /* else just set */
    gui->visible = true;
    gui->updated_visible = true;
}
static void _common_update_visible()
{
    Gui *gui;

    entitypool_foreach(gui, gui_pool)
        gui->updated_visible = false;
    entitypool_foreach(gui, gui_pool)
        _common_update_visible_rec(gui);
}

static void _common_reset_align()
{
    Gui *gui;
    Vec2 pos;

    entitypool_foreach(gui, gui_pool)
    {
        pos = transform_get_position(gui->pool_elem.ent);
        if (gui->halign != GA_NONE)
            pos.x = gui->padding.x;
        if (gui->valign != GA_NONE)
            pos.y = -gui->padding.y;
        transform_set_position(gui->pool_elem.ent, pos);
    }
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
            default:                                                    \
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

static void _common_mouse_event(EntityMap *emap, MouseCode mouse)
{
    Gui *gui;
    Vec2 m;
    Mat3 t;
    Entity ent;
    bool some_focused = false;

    m = camera_unit_to_world(input_get_mouse_pos_unit());
    entitypool_foreach(gui, gui_pool)
        if (gui->visible
            && !(edit_get_enabled() && edit_get_editable(gui->pool_elem.ent)))
        {
            ent = gui->pool_elem.ent;

            t = mat3_inverse(transform_get_world_matrix(ent));
            if (bbox_contains(gui->bbox, mat3_transform(t, m)))
            {
                entitymap_set(emap, ent, mouse);

                /* focus? */
                if (gui->focusable && mouse == MC_LEFT)
                {
                    some_focused = true;
                    gui_set_focused_entity(ent);
                }
            }
        }

    /* none focused? clear */
    if (!some_focused)
        gui_set_focused_entity(entity_nil);
}
static void _common_mouse_down(MouseCode mouse)
{
    _common_mouse_event(mouse_down_map, mouse);
}
static void _common_mouse_up(MouseCode mouse)
{
    _common_mouse_event(mouse_up_map, mouse);
}
static void _common_event_clear()
{
    entitymap_clear(focus_enter_map);
    entitymap_clear(focus_exit_map);
    entitymap_clear(mouse_down_map);
    entitymap_clear(mouse_up_map);
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
        bool_save(&gui->visible, s);
        bool_save(&gui->setvisible, s);
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
        bool_load(&gui->visible, s);
        bool_load(&gui->setvisible, s);
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
    bool visible;
    Color color;

    bool hfit;
    bool vfit;
    bool hfill;
    bool vfill;

    bool updated;
    int depth; /* for draw order -- child depth > parent depth */
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
    rect->hfit = true;
    rect->vfit = true;
    rect->hfill = false;
    rect->vfill = false;
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

void gui_rect_set_hfit(Entity ent, bool fit)
{
    Rect *rect = entitypool_get(rect_pool, ent);
    assert(rect);
    rect->hfit = fit;
}
bool gui_rect_get_hfit(Entity ent)
{
    Rect *rect = entitypool_get(rect_pool, ent);
    assert(rect);
    return rect->hfit;
}
void gui_rect_set_vfit(Entity ent, bool fit)
{
    Rect *rect = entitypool_get(rect_pool, ent);
    assert(rect);
    rect->vfit = fit;
}
bool gui_rect_get_vfit(Entity ent)
{
    Rect *rect = entitypool_get(rect_pool, ent);
    assert(rect);
    return rect->vfit;
}

void gui_rect_set_hfill(Entity ent, bool fill)
{
    Rect *rect = entitypool_get(rect_pool, ent);
    assert(rect);
    rect->hfill = fill;
}
bool gui_rect_get_hfill(Entity ent)
{
    Rect *rect = entitypool_get(rect_pool, ent);
    assert(rect);
    return rect->hfill;
}
void gui_rect_set_vfill(Entity ent, bool fill)
{
    Rect *rect = entitypool_get(rect_pool, ent);
    assert(rect);
    rect->vfill = fill;
}
bool gui_rect_get_vfill(Entity ent)
{
    Rect *rect = entitypool_get(rect_pool, ent);
    assert(rect);
    return rect->vfill;
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
    gfx_bind_vertex_attrib(rect_program, GL_INT, 1, "visible", Rect, visible);
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

static void _rect_update_child_first(Entity ent);

static void _rect_update_table_align(Rect *rect)
{
    Entity rect_ent, *children;
    Gui *child;
    unsigned int nchildren, i;
    Scalar delta;
    BBox b;
    Vec2 pos, curr;

    rect_ent = rect->pool_elem.ent;

    curr = vec2_zero;
    children = transform_get_children(rect_ent);
    nchildren = transform_get_num_children(rect_ent);
    for (i = 0; i < nchildren; ++i)
    {
        child = entitypool_get(gui_pool, children[i]);
        if (!(child && child->visible
              && (child->halign == GA_TABLE || child->valign == GA_TABLE)))
            continue;
        _rect_update_child_first(children[i]);

        b = bbox_transform(transform_get_matrix(children[i]), child->bbox);
        pos = transform_get_position(children[i]);

        if (child->halign == GA_TABLE)
        {
            delta = curr.x + child->padding.x - b.min.x;
            pos.x += delta;
            curr.x = b.max.x + delta;
        }
        if (child->valign == GA_TABLE)
        {
            delta = curr.y - child->padding.y - b.max.y;
            pos.y += delta;
            curr.y = b.min.y + delta;
        }

        transform_set_position(children[i], pos);
    }
}

static void _rect_update_fit(Rect *rect)
{
    Entity rect_ent, *children;
    Gui *child;
    unsigned int nchildren, i;
    Scalar miny, maxx;
    BBox b;

    /* we also fit hfill/vfill rects, then fill them after we're all done */

    rect_ent = rect->pool_elem.ent;

    miny = 0;
    maxx = 0;

    children = transform_get_children(rect_ent);
    nchildren = transform_get_num_children(rect_ent);
    for (i = 0; i < nchildren; ++i)
    {
        child = entitypool_get(gui_pool, children[i]);
        if (!child || !child->visible)
            continue;
        _rect_update_child_first(children[i]);

        b = bbox_transform(transform_get_matrix(children[i]), child->bbox);
        if (rect->hfit || rect->hfill)
            maxx = scalar_max(maxx, b.max.x + child->padding.x);
        if (rect->vfit || rect->vfill)
            miny = scalar_min(miny, b.min.y - child->padding.y);
    }

    if (rect->hfit || rect->hfill)
        rect->size.x = maxx;
    if (rect->vfit || rect->vfill)
        rect->size.y = -miny;
}

static void _rect_update_child_first(Entity ent)
{
    Rect *rect;
    Gui *gui;

    gui = entitypool_get(gui_pool, ent);
    if (!gui)
        return;

    rect = entitypool_get(rect_pool, ent);
    if (!rect || rect->updated)
        return;
    _rect_update_table_align(rect);
    _rect_update_fit(rect);

    gui->bbox = bbox_bound(vec2_zero, vec2(rect->size.x, -rect->size.y));
}

static void _rect_update_parent_first(Entity ent);

static void _rect_update_fill(Rect *rect)
{
    Entity ent;
    Gui *pgui, *gui;
    BBox b;
    Entity parent;

    ent = rect->pool_elem.ent;
    gui = entitypool_get(gui_pool, ent);
    if (!gui)
        return;

    if (!rect || !rect->visible || rect->updated || !(rect->hfill || rect->vfill))
        return;

    parent = transform_get_parent(ent);
    pgui = entitypool_get(gui_pool, parent);
    if (!pgui)
        return; /* no parent to fill to */

    _rect_update_parent_first(parent);
    b = bbox_transform(mat3_inverse(transform_get_matrix(ent)), pgui->bbox);

    if (rect->hfill)
        rect->size.x = b.max.x - gui->padding.x;
    if (rect->vfill)
        rect->size.y = -b.min.y + gui->padding.y;
}

static void _rect_update_depth(Rect *rect)
{
    Rect *prect;

    prect = entitypool_get(rect_pool, transform_get_parent(rect->pool_elem.ent));
    if (prect)
    {
        _rect_update_parent_first(prect->pool_elem.ent);
        rect->depth = prect->depth + 1;
    }
    else
        rect->depth = 0;
}

static void _rect_update_parent_first(Entity ent)
{
    Rect *rect;
    Gui *gui;

    gui = entitypool_get(gui_pool, ent);
    if (!gui)
        return;

    rect = entitypool_get(rect_pool, ent);
    if (!rect || rect->updated)
        return;
    _rect_update_fill(rect);
    _rect_update_depth(rect);

    gui->bbox = bbox_bound(vec2_zero, vec2(rect->size.x, -rect->size.y));
}

static void _rect_update_all()
{
    Rect *rect;
    Gui *gui;

    entitypool_remove_destroyed(rect_pool, gui_rect_remove);

    entitypool_foreach(rect, rect_pool)
        rect->updated = false;
    entitypool_foreach(rect, rect_pool)
        _rect_update_child_first(rect->pool_elem.ent);

    entitypool_foreach(rect, rect_pool)
        rect->updated = false;
    entitypool_foreach(rect, rect_pool)
        _rect_update_parent_first(rect->pool_elem.ent);

    entitypool_foreach(rect, rect_pool)
    {
        gui = entitypool_get(gui_pool, rect->pool_elem.ent);
        assert(gui);

        /* write gui bbox */
        gui->bbox = bbox_bound(vec2_zero, vec2(rect->size.x, -rect->size.y));

        /* read gui properties */
        rect->visible = gui->visible;
        rect->color = gui->color;
    }
}

static void _rect_update_wmat()
{
    Rect *rect;
    entitypool_foreach(rect, rect_pool)
        rect->wmat = transform_get_world_matrix(rect->pool_elem.ent);
}

static int _rect_depth_compare(const void *a, const void *b)
{
    const Rect *ra = a, *rb = b;
    if (ra->depth == rb->depth)
        return ((int) ra->pool_elem.ent.id) - ((int) rb->pool_elem.ent.id);
    return ra->depth - rb->depth;
}

static void _rect_draw_all()
{
    unsigned int nrects;

    /* depth sort */
    entitypool_sort(rect_pool, _rect_depth_compare);

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
        bool_save(&rect->hfit, s);
        bool_save(&rect->vfit, s);
        bool_save(&rect->hfill, s);
        bool_save(&rect->vfill, s);
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
        bool_load(&rect->hfit, s);
        bool_load(&rect->vfit, s);
        bool_load(&rect->hfill, s);
        bool_load(&rect->vfill, s);
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
    Vec2 pos;        /* position in space of text entity in size-less units */
    Vec2 cell;       /* cell in font image */
    float is_cursor; /* > 0 iff. this char is cursor */
};

/* info per text entity */
typedef struct Text Text;
struct Text
{
    EntityPoolElem pool_elem;

    char *str;
    Array *chars;  /* per-character info buffered to shader */
    Vec2 bounds;   /* max x, min y in size-less units */

    int cursor;
};

static EntityPool *text_pool;

static Scalar cursor_blink_time = 0;

static void _text_add_cursor(Text *text, Vec2 pos)
{
    TextChar *tc;

    /* compute position in font grid */
    tc = array_add(text->chars);
    tc->pos = pos;
    tc->cell = vec2(' ' % TEXT_GRID_W, TEXT_GRID_H - 1 - (' ' / TEXT_GRID_W));
    tc->is_cursor = 1;
}

static void _text_set_str(Text *text, const char *str)
{
    char c;
    TextChar *tc;
    Vec2 pos;
    int i = 0;

    /* copy to struct? */
    if (str)
    {
        free(text->str);
        text->str = malloc(strlen(str) + 1);
        strcpy(text->str, str);
    }
    else
    {
        str = text->str;
    }

    /* create TextChar array and update bounds */
    pos = vec2(0, -1);
    text->bounds = vec2(1, -1);
    array_clear(text->chars);
    while (*str)
    {
        if (i++ == text->cursor)
            _text_add_cursor(text, pos);

        c = *str++;
        switch (c)
        {
            case '\n':
                /* next line */
                pos.x = 0;
                pos.y -= 1;
                continue;
        }

        /* compute position in font grid */
        tc = array_add(text->chars);
        tc->pos = pos;
        tc->cell = vec2(c % TEXT_GRID_W, TEXT_GRID_H - 1 - (c / TEXT_GRID_W));
        tc->is_cursor = -1;

        /* move ahead */
        pos.x += 1;
        text->bounds.x = scalar_max(text->bounds.x, pos.x);
    }

    /* cursor at end? */
    if (i == text->cursor)
    {
        _text_add_cursor(text, pos);
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
    text->cursor = -1;
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

void gui_text_set_cursor(Entity ent, int cursor)
{
    Text *text = entitypool_get(text_pool, ent);
    assert(text);
    text->cursor = cursor;
    _text_set_str(text, NULL);
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
    gfx_bind_vertex_attrib(text_program, GL_FLOAT, 1, "is_cursor",
                           TextChar, is_cursor);
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

    cursor_blink_time += 2 * timing_true_dt;

    entitypool_remove_destroyed(text_pool, gui_text_remove);

    entitypool_foreach(text, text_pool)
    {
        /* blink on when focus entered */
        if (gui_event_focus_enter(text->pool_elem.ent))
            cursor_blink_time = 1;

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
    glUniform1f(glGetUniformLocation(text_program, "cursor_blink"),
                ((int) cursor_blink_time) & 1);
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
        gui = entitypool_get(gui_pool, text->pool_elem.ent);
        assert(gui);
        if (!gui->visible)
            continue;
        glUniform4fv(glGetUniformLocation(text_program, "base_color"), 1,
                     (const GLfloat *) &gui->color);

        wmat = transform_get_world_matrix(text->pool_elem.ent);
        glUniformMatrix3fv(glGetUniformLocation(text_program, "wmat"),
                           1, GL_FALSE, (const GLfloat *) &wmat);

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

/* --- textedit ------------------------------------------------------------ */

typedef struct TextEdit TextEdit;
struct TextEdit
{
    EntityPoolElem pool_elem;

    unsigned int cursor; /* 0 at beginning of string */
};

EntityPool *textedit_pool;

void gui_textedit_add(Entity ent)
{
    TextEdit *textedit;

    if (entitypool_get(textedit_pool, ent))
        return;

    gui_text_add(ent);
    gui_set_focusable(ent, true);

    textedit = entitypool_add(textedit_pool, ent);
    textedit->cursor = 0;
}

void gui_textedit_remove(Entity ent)
{
    entitypool_remove(textedit_pool, ent);
}

static void _textedit_init()
{
    textedit_pool = entitypool_new(TextEdit);
}
static void _textedit_deinit()
{
    entitypool_free(textedit_pool);
}

static void _textedit_key_down(KeyCode key)
{
    Entity ent;
    TextEdit *textedit;
    char c;
    const char *old;
    char *new = NULL;

    textedit = entitypool_get(textedit_pool, focused);
    if (!textedit)
        return;
    ent = textedit->pool_elem.ent;

    /* blink on for feedback */
    cursor_blink_time = 1;

    old = gui_text_get_str(ent);

    /* confirm? */
    if (key == KC_ENTER
        && (input_key_down(KC_LEFT_SHIFT)
            || input_key_down(KC_RIGHT_SHIFT)
            || input_key_down(KC_LEFT_CONTROL)
            || input_key_down(KC_RIGHT_CONTROL)))
    {
        gui_set_focused_entity(entity_nil);
    }

    /* left/right */
    else if (key == KC_LEFT)
    {
        if (textedit->cursor > 0)
            --textedit->cursor;
    }
    else if (key == KC_RIGHT)
    {
        if (textedit->cursor < strlen(old))
            ++textedit->cursor;
    }

    /* remove char */
    else if (key == KC_BACKSPACE || key == KC_DELETE)
    {
        if (key == KC_BACKSPACE)
            if (textedit->cursor > 0)
                --textedit->cursor;

        new = malloc(strlen(old)); /* 1 less, but 1 more for null */
        strncpy(new, old, textedit->cursor);
        strcpy(&new[textedit->cursor], &old[textedit->cursor + 1]);
        gui_text_set_str(ent, new);
    }

    /* insert char */
    else if (key == KC_ENTER || input_keycode_is_char(key))
    {
        if (input_keycode_is_char(key))
            c = input_keycode_to_char(key);
        else
            c = '\n';

        new = malloc(strlen(old) + 2); /* 1 for new char, 1 for null */
        strncpy(new, old, textedit->cursor);
        new[textedit->cursor] = c;
        strcpy(&new[textedit->cursor + 1], &old[textedit->cursor]);
        gui_text_set_str(ent, new);
        ++textedit->cursor;
    }

    free(new);
}

static void _textedit_update_all()
{
    Entity ent;
    TextEdit *textedit;

    entitypool_remove_destroyed(textedit_pool, gui_textedit_remove);

    entitypool_foreach(textedit, textedit_pool)
    {
        ent = textedit->pool_elem.ent;

        /* focus stuff */
        if (gui_get_focus(ent))
            gui_text_set_cursor(ent, textedit->cursor);
        else
            gui_text_set_cursor(ent, -1);
    }
}

static void _textedit_save_all(Serializer *s)
{
    TextEdit *textedit;

    entitypool_foreach(textedit, textedit_pool)
    {
        if (!entity_get_save_filter(textedit->pool_elem.ent))
            continue;
        loop_continue_save(s);

        entitypool_elem_save(textedit_pool, &textedit, s);

        uint_save(&textedit->cursor, s);
    }
    loop_end_save(s);
}
static void _textedit_load_all(Deserializer *s)
{
    TextEdit *textedit;

    while (loop_continue_load(s))
    {
        entitypool_elem_load(textedit_pool, &textedit, s);

        uint_load(&textedit->cursor, s);
    }
}

/* ------------------------------------------------------------------------- */

void gui_event_clear()
{
    _common_event_clear();
}

static void _create_root()
{
    gui_root = entity_create();
    transform_add(gui_root);
    transform_set_position(gui_root, vec2(-1, 1)); /* origin at top-left */
    gui_rect_add(gui_root);
    gui_rect_set_hfit(gui_root, false);
    gui_rect_set_vfit(gui_root, false);
    gui_set_color(gui_root, color(0, 0, 0, 0));
}

void gui_init()
{
    focused = entity_nil;
    _common_init();
    _rect_init();
    _text_init();
    _textedit_init();
    _create_root();
}
void gui_deinit()
{
    _textedit_deinit();
    _text_deinit();
    _rect_deinit();
    _common_deinit();
}

static void _update_root()
{
    Vec2 win_size;

    win_size = game_get_window_size();

    edit_set_editable(gui_root, false);

    /* child of camera so GUI stays on screen */
    transform_set_parent(gui_root, camera_get());

    /* use pixel coordinates */
    transform_set_scale(gui_root, scalar_vec2_div(2, win_size));
    gui_rect_set_size(gui_root, win_size);
}

void gui_update_all()
{
    _update_root();
    _common_update_destroyed();
    _common_update_visible();
    _common_reset_align();
    _textedit_update_all();
    _text_update_all();
    _rect_update_all();
    _common_update_align();
    _rect_update_wmat();
    _common_update_all();
}

void gui_draw_all()
{
    _rect_draw_all();
    _text_draw_all();
}

void gui_key_down(KeyCode key)
{
    _textedit_key_down(key);
}
void gui_key_up(KeyCode key)
{
}
void gui_mouse_down(MouseCode mouse)
{
    _common_mouse_down(mouse);
}
void gui_mouse_up(MouseCode mouse)
{
    _common_mouse_up(mouse);
}

void gui_save_all(Serializer *s)
{
    _common_save_all(s);
    _rect_save_all(s);
    _text_save_all(s);
    _textedit_save_all(s);
}
void gui_load_all(Deserializer *s)
{
    _common_load_all(s);
    _rect_load_all(s);
    _text_load_all(s);
    _textedit_load_all(s);
}
