#include "edit.h"

#include "gfx.h"
#include "entitypool.h"
#include "mat3.h"
#include "transform.h"
#include "camera.h"
#include "dirs.h"
#include "input.h"
#include "game.h"

typedef enum Mode Mode;
enum Mode
{
    MD_DISABLED,
    MD_NORMAL,

    MD_GRAB,
    MD_ROTATE,
};

Mode mode = MD_DISABLED;

typedef struct BBoxPoolElem BBoxPoolElem;
struct BBoxPoolElem
{
    EntityPoolElem pool_elem;

    Mat3 wmat;
    BBox bbox;
    Scalar selected; /* > 0.5 if and only if selected */
};

static EntityPool *bbox_pool;

typedef struct SelectPoolElem SelectPoolElem;
struct SelectPoolElem
{
    EntityPoolElem pool_elem;
};

static EntityPool *select_pool;

Vec2 mouse_prev;
Vec2 mouse_curr;

/* ------------------------------------------------------------------------- */

void edit_set_enabled(bool e)
{
    mode = e ? MD_NORMAL : MD_DISABLED;
}
bool edit_get_enabled()
{
    return mode != MD_DISABLED;
}

void edit_clear_bboxes()
{
    entitypool_clear(bbox_pool);
}
void edit_update_bbox(Entity ent, BBox bbox)
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

static void _keydown(KeyCode key)
{
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

                default:
                    break;
            }

        default:
            break;
    };
}

static void _mousedown(MouseCode mouse)
{
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

    mouse_curr = input_get_mouse_pos_unit();

    /* mode-specific update */
    switch (mode)
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
        elem->selected = edit_select_has(ent) ? 1 : 0;
    }
}

void edit_draw_all()
{
    unsigned int nbboxes;
    Vec2 win;

    if (mode == MD_DISABLED)
        return;

    /* update bbox world matrices */

    /* bind program, update uniforms */
    glUseProgram(program);
    glUniformMatrix3fv(glGetUniformLocation(program, "inverse_view_matrix"),
                       1, GL_FALSE,
                       (const GLfloat *) camera_get_inverse_view_matrix_ptr());
    win = game_get_window_size();
    glUniform1f(glGetUniformLocation(program, "aspect"),
                win.x / win.y);

    /* draw! */
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    nbboxes = entitypool_size(bbox_pool);
    glBufferData(GL_ARRAY_BUFFER, nbboxes * sizeof(BBoxPoolElem),
                 entitypool_begin(bbox_pool), GL_STREAM_DRAW);
    glDrawArrays(GL_POINTS, 0, nbboxes);
}
