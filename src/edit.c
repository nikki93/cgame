#include "edit.h"

#include <assert.h>
#include <stdio.h>

#include "gfx.h"
#include "entitymap.h"
#include "entitypool.h"
#include "mat3.h"
#include "transform.h"
#include "camera.h"
#include "dirs.h"
#include "input.h"
#include "game.h"

static bool enabled;

/* editability data */
static EntityPool *uneditable_pool; /* Entites are in this pool
                                       iff. not editable */

/* grid */
static Vec2 grid_size = { 2.0, 2.0 };

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
    enabled = e;
}
bool edit_get_enabled()
{
    return enabled;
}

void edit_set_grid_size(Vec2 size)
{
    if (size.x < 0.0)
        size.x = 0.0;
    if (size.y < 0.0)
        size.y = 0.0;

    grid_size = size;
}
Vec2 edit_get_grid_size()
{
    return grid_size;
}

void edit_set_editable(Entity ent, bool editable)
{
    if (editable)
        entitypool_remove(uneditable_pool, ent);
    else
        entitypool_add(uneditable_pool, ent);
}
bool edit_get_editable(Entity ent)
{
    return !entitypool_get(uneditable_pool, ent);
}

void edit_bboxes_clear()
{
    entitypool_clear(bbox_pool);
}
void edit_bboxes_update(Entity ent, BBox bbox)
{
    BBoxPoolElem *elem;

    /* editable? */
    if (!edit_get_editable(ent))
        return;

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

void edit_init()
{
    /* init pools/maps */
    bbox_pool = entitypool_new(BBoxPoolElem);
    select_pool = entitypool_new(SelectPoolElem);
    uneditable_pool = entitypool_new(EntityPoolElem);

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

    /* deinit pools/maps */
    entitypool_free(uneditable_pool);
    entitypool_free(select_pool);
    entitypool_free(bbox_pool);
}

static void _uneditable_remove(Entity ent)
{
    entitypool_remove(uneditable_pool, ent);
}

void edit_update_all()
{
    BBoxPoolElem *elem;
    Entity ent;

    entitypool_remove_destroyed(uneditable_pool, _uneditable_remove);

    if (!enabled)
        return;

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

void edit_draw_all()
{
    if (!enabled)
        return;

    _bind_bbox_program();
    _draw_bboxes();
}

void edit_save_all(Serializer *s)
{
    EntityPoolElem *elem;
    entitypool_save_foreach(elem, uneditable_pool, s);
}
void edit_load_all(Deserializer *s)
{
    EntityPoolElem *elem;
    entitypool_load_foreach(elem, uneditable_pool, s);
}
