#include "edit.h"

#include <assert.h>
#include <stdio.h>

#include "gfx.h"
#include "entitypool.h"
#include "mat3.h"
#include "transform.h"
#include "camera.h"
#include "dirs.h"
#include "input.h"
#include "game.h"

/* modes */
typedef enum Mode Mode;
enum Mode
{
    MD_DISABLED,
    MD_NORMAL,

    MD_BOX_SELECT,

    MD_GRAB,
    MD_ROTATE,
};
Mode mode = MD_DISABLED;

/* bbox pool */
typedef struct BBoxPoolElem BBoxPoolElem;
struct BBoxPoolElem
{
    EntityPoolElem pool_elem;

    Mat3 wmat;
    BBox bbox;
    Scalar selected; /* > 0.5 if and only if selected */
};
static EntityPool *bbox_pool;

/* select pool */
typedef struct SelectPoolElem SelectPoolElem;
struct SelectPoolElem
{
    EntityPoolElem pool_elem;
};
static EntityPool *select_pool;

/* mouse position */
Vec2 mouse_prev;
Vec2 mouse_curr;

/* box select */
bool boxsel_started;
Vec2 boxsel_start;

/* ------------------------------------------------------------------------- */

void edit_set_enabled(bool e)
{
    mode = e ? MD_NORMAL : MD_DISABLED;
}
bool edit_get_enabled()
{
    return mode != MD_DISABLED;
}

void edit_bboxes_clear()
{
    entitypool_clear(bbox_pool);
}
void edit_bboxes_update(Entity ent, BBox bbox)
{
    BBoxPoolElem *elem;

    elem = entitypool_get(bbox_pool, ent);

    /* merge if already exists, else set */
    if (elem)
        elem->bbox = bbox_merge(elem->bbox, bbox);
    else
    {
        elem = entitypool_add(bbox_pool, ent);
        elem->bbox = bbox;
    }
}

unsigned int edit_bboxes_get_num()
{
    return entitypool_size(bbox_pool);
}
EntityBBoxPair edit_bboxes_get_nth(unsigned int n)
{
    BBoxPoolElem *elem;
    EntityBBoxPair pair;

    assert(n < entitypool_size(bbox_pool));
    elem = entitypool_nth(bbox_pool, n);

    pair.ent = elem->pool_elem.ent;
    pair.bbox = elem->bbox;
    return pair;
}

void edit_bboxes_set_selected(Entity ent, bool selected)
{
    BBoxPoolElem *elem = entitypool_get(bbox_pool, ent);
    assert(elem);
    elem->selected = selected;
}

void edit_select_clear()
{
    entitypool_clear(select_pool);
}
void edit_select_add(Entity ent)
{
    if (!edit_select_has(ent))
        entitypool_add(select_pool, ent);
}
void edit_select_remove(Entity ent)
{
    entitypool_remove(select_pool, ent);
}
bool edit_select_has(Entity ent)
{
    return entitypool_get(select_pool, ent) != NULL;
}

/* ------------------------------------------------------------------------- */

static GLuint program;
static GLuint vao;
static GLuint vbo;

static void _select_click()
{
    BBoxPoolElem *elem;
    Entity ent, first = entity_nil, last = entity_nil;
    Mat3 t;
    Vec2 m, p;
    bool prev_select = false;

    m = camera_unit_to_world(mouse_curr);

    /* look for entities whose bbox contains m */
    entitypool_foreach(elem, bbox_pool)
    {
        ent = elem->pool_elem.ent;

        /* transform m into entity space */
        t = transform_get_world_matrix(ent);
        t = mat3_inverse(t);
        p = mat3_transform(t, m);

        /* if clicked, select */
        if (bbox_contains(elem->bbox, p))
        {
            if (entity_eq(first, entity_nil))
                first = ent;
            last = ent;

            if (!input_key_down(KC_LEFT_CONTROL)
                && !input_key_down(KC_RIGHT_CONTROL))
            {
                if (prev_select)
                {
                    prev_select = false;
                    break; /* previous was selected, single-select mode,
                              stop here */
                }

                prev_select = edit_select_has(ent);
                edit_select_add(ent);
            }
            else if (edit_select_has(ent))
                edit_select_remove(ent);
            else
                edit_select_add(ent);
        }
    }

    /* single-select */
    if (!input_key_down(KC_LEFT_CONTROL)
        && !input_key_down(KC_RIGHT_CONTROL))
    {
        edit_select_clear();

        if (prev_select)
            last = first; /* last was selected, loop to first */
        if (!entity_eq(last, entity_nil))
            edit_select_add(last);
    }
}

static void _destroy_selected()
{
    SelectPoolElem *elem;
    entitypool_foreach(elem, select_pool)
        entity_destroy(elem->pool_elem.ent);
    edit_select_clear();
}

static void _boxsel()
{
    BBoxPoolElem *elem;
    Entity ent;
    Vec2 o, m;
    BBox box;

    if (!input_key_down(KC_LEFT_CONTROL)
        && !input_key_down(KC_RIGHT_CONTROL))
        edit_select_clear();

    m = camera_unit_to_world(input_get_mouse_pos_unit());
    box = bbox_merge(bbox(boxsel_start, boxsel_start), bbox(m, m));

    entitypool_foreach(elem, bbox_pool)
    {
        ent = elem->pool_elem.ent;
        o = mat3_transform(transform_get_world_matrix(ent), vec2_zero);
        if (bbox_contains(box, o))
            edit_select_add(ent);
    }
}

static void _keydown(KeyCode key)
{
    return;
    
    switch (mode)
    {
        case MD_NORMAL:
            switch (key)
            {
                case KC_D:
                    _destroy_selected();
                    break;

                case KC_G:
                    mouse_prev = input_get_mouse_pos_unit();
                    mode = MD_GRAB;
                    break;

                case KC_R:
                    mouse_prev = input_get_mouse_pos_unit();
                    mode = MD_ROTATE;
                    break;

                case KC_B:
                    mode = MD_BOX_SELECT;
                    boxsel_started = false;
                    break;

                default:
                    break;
            }

        default:
            break;
    };
}

static void _mousedown(MouseCode mouse)
{
    return;
    
    switch (mode)
    {
        case MD_NORMAL:
            switch (mouse)
            {
                case MC_LEFT:
                    _select_click();
                    break;

                default:
                    break;
            }
            break;

        case MD_GRAB:
            switch (mouse)
            {
                case MC_LEFT:
                    mode = MD_NORMAL;
                    break;

                default:
                    break;
            }

        case MD_ROTATE:
            switch (mouse)
            {
                case MC_LEFT:
                    mode = MD_NORMAL;
                    break;

                default:
                    break;
            }

        case MD_BOX_SELECT:
            if (boxsel_started)
                break;
            boxsel_start = camera_unit_to_world(input_get_mouse_pos_unit());
            boxsel_started = true;
            break;

        default:
            break;
    }
}

static void _mouseup(MouseCode mouse)
{
    return;
    
    switch (mode)
    {
        case MD_BOX_SELECT:
            _boxsel();
            mode = MD_NORMAL;
            break;

        default:
            break;
    }
}

void edit_init()
{
    /* init pools */
    bbox_pool = entitypool_new(BBoxPoolElem);
    select_pool = entitypool_new(SelectPoolElem);

    /* bind callbacks */
    input_add_key_down_callback(_keydown);
    input_add_mouse_down_callback(_mousedown);
    input_add_mouse_up_callback(_mouseup);

    /* create shader program, load atlas, bind parameters */
    program = gfx_create_program(data_path("bbox.vert"),
                                 data_path("bbox.geom"),
                                 data_path("bbox.frag"));
    glUseProgram(program);

    /* make vao, vbo, bind attributes */
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    gfx_bind_vertex_attrib(program, GL_FLOAT, 3, "wmat1",
                           BBoxPoolElem, wmat.m[0]);
    gfx_bind_vertex_attrib(program, GL_FLOAT, 3, "wmat2",
                           BBoxPoolElem, wmat.m[1]);
    gfx_bind_vertex_attrib(program, GL_FLOAT, 3, "wmat3",
                           BBoxPoolElem, wmat.m[2]);
    gfx_bind_vertex_attrib(program, GL_FLOAT, 2, "bbmin",
                           BBoxPoolElem, bbox.min);
    gfx_bind_vertex_attrib(program, GL_FLOAT, 2, "bbmax",
                           BBoxPoolElem, bbox.max);
    gfx_bind_vertex_attrib(program, GL_FLOAT, 1, "selected",
                           BBoxPoolElem, selected);
}
void edit_deinit()
{
    /* clean up GL stuff */
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    /* deinit pools */
    entitypool_free(select_pool);
    entitypool_free(bbox_pool);
}

static void _update_grab()
{
    SelectPoolElem *elem;
    Vec2 trans, mc, mp;
    Mat3 m;
    Entity ent, parent, anc;

    mc = camera_unit_to_world(mouse_curr);
    mp = camera_unit_to_world(mouse_prev);

    entitypool_foreach(elem, select_pool)
    {
        ent = elem->pool_elem.ent;

        /* if an ancestor has been or will be moved, ignore */
        for (anc = transform_get_parent(ent); !entity_eq(anc, entity_nil);
             anc = transform_get_parent(anc))
            if (edit_select_has(anc))
                goto ignore;

        /* account for parent-space coords -- grab must be world-space */
        if (!entity_eq(parent = transform_get_parent(ent), entity_nil))
            m = mat3_inverse(transform_get_world_matrix(parent));
        else
            m = mat3_identity();
        trans = vec2_sub(mat3_transform(m, mc), mat3_transform(m, mp));
        transform_translate(ent, trans);

    ignore: ;
    }

    mouse_prev = mouse_curr;
}

static void _update_rotate()
{
    SelectPoolElem *elem;
    Vec2 mc, mp, world_pos;
    Scalar rot;
    Entity ent, anc;

    mc = camera_unit_to_world(mouse_curr);
    mp = camera_unit_to_world(mouse_prev);

    entitypool_foreach(elem, select_pool)
    {
        ent = elem->pool_elem.ent;

        /* if an ancestor has been or will be moved, ignore */
        for (anc = transform_get_parent(ent); !entity_eq(anc, entity_nil);
             anc = transform_get_parent(anc))
            if (edit_select_has(anc))
                goto ignore;

        /* rotate around entity origin */
        world_pos = mat3_transform(transform_get_world_matrix(ent),
                                   vec2_zero);
        rot = vec2_atan2(vec2_sub(mp, world_pos))
            - vec2_atan2(vec2_sub(mc, world_pos));
        transform_rotate(ent, rot);

    ignore: ;
    }

    mouse_prev = mouse_curr;
}

void edit_update_all()
{
    BBoxPoolElem *elem;
    Entity ent;

    if (mode == MD_DISABLED)
        return;

    /* mode-specific update */
    switch (22)
    {
        case MD_GRAB:
            _update_grab();
            break;

        case MD_ROTATE:
            _update_rotate();
            break;

        default:
            break;
    }

    /* update bbox world matrices */
    entitypool_foreach(elem, bbox_pool)
    {
        ent = elem->pool_elem.ent;
        elem->wmat = transform_get_world_matrix(ent);
    }
}

static void _bind_bbox_program()
{
    Vec2 win;

    glUseProgram(program);
    glUniformMatrix3fv(glGetUniformLocation(program, "inverse_view_matrix"),
                       1, GL_FALSE,
                       (const GLfloat *) camera_get_inverse_view_matrix_ptr());
    win = game_get_window_size();
    glUniform1f(glGetUniformLocation(program, "aspect"),
                win.x / win.y);
}

/* needs bbox shader program to be bound */
static void _draw_bboxes()
{
    unsigned int nbboxes;

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    nbboxes = entitypool_size(bbox_pool);
    glBufferData(GL_ARRAY_BUFFER, nbboxes * sizeof(BBoxPoolElem),
                 entitypool_begin(bbox_pool), GL_STREAM_DRAW);
    glDrawArrays(GL_POINTS, 0, nbboxes);
}

/* needs bbox shader program to be bound */
static void _draw_boxsel()
{
    Vec2 m;
    BBoxPoolElem elem;

    if (!boxsel_started)
        return;

    /* piggyback off of bbox drawing */

    elem.wmat = mat3_identity();

    m = camera_unit_to_world(input_get_mouse_pos_unit());
    elem.bbox = bbox_merge(bbox(boxsel_start, boxsel_start), bbox(m, m));
    elem.selected = false;

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(elem), &elem, GL_STREAM_DRAW);
    glDrawArrays(GL_POINTS, 0, 1);
}

void edit_draw_all()
{
    if (mode == MD_DISABLED)
        return;

    _bind_bbox_program();
    _draw_bboxes();
    if (mode == MD_BOX_SELECT)
        _draw_boxsel();
}
