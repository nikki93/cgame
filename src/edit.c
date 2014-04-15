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
#include "array.h"

static bool enabled;

/* editability data */
static EntityPool *uneditable_pool; /* Entites are in this pool
                                       iff. not editable */

void edit_set_enabled(bool e)
{
    enabled = e;
}
bool edit_get_enabled()
{
    return enabled;
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

/* --- bboxes -------------------------------------------------------------- */

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

static GLuint bboxes_program;
static GLuint bboxes_vao;
static GLuint bboxes_vbo;

static void _bboxes_init()
{
    bbox_pool = entitypool_new(BBoxPoolElem);

    /* create shader program, load atlas, bind parameters */
    bboxes_program = gfx_create_program(data_path("bbox.vert"),
                                        data_path("bbox.geom"),
                                        data_path("bbox.frag"));
    glUseProgram(bboxes_program);

    /* make vao, vbo, bind attributes */
    glGenVertexArrays(1, &bboxes_vao);
    glBindVertexArray(bboxes_vao);
    glGenBuffers(1, &bboxes_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, bboxes_vbo);
    gfx_bind_vertex_attrib(bboxes_program, GL_FLOAT, 3, "wmat1",
                           BBoxPoolElem, wmat.m[0]);
    gfx_bind_vertex_attrib(bboxes_program, GL_FLOAT, 3, "wmat2",
                           BBoxPoolElem, wmat.m[1]);
    gfx_bind_vertex_attrib(bboxes_program, GL_FLOAT, 3, "wmat3",
                           BBoxPoolElem, wmat.m[2]);
    gfx_bind_vertex_attrib(bboxes_program, GL_FLOAT, 2, "bbmin",
                           BBoxPoolElem, bbox.min);
    gfx_bind_vertex_attrib(bboxes_program, GL_FLOAT, 2, "bbmax",
                           BBoxPoolElem, bbox.max);
    gfx_bind_vertex_attrib(bboxes_program, GL_FLOAT, 1, "selected",
                           BBoxPoolElem, selected);
}
static void _bboxes_deinit()
{
    /* clean up GL stuff */
    glDeleteProgram(bboxes_program);
    glDeleteBuffers(1, &bboxes_vbo);
    glDeleteVertexArrays(1, &bboxes_vao);

    entitypool_free(bbox_pool);
}

static void _bboxes_update_all()
{
    Entity ent;
    BBoxPoolElem *elem;

    if (!enabled)
        return;

    /* update bbox world matrices */
    entitypool_foreach(elem, bbox_pool)
    {
        ent = elem->pool_elem.ent;
        elem->wmat = transform_get_world_matrix(ent);
    }
}

/* needs bbox shader program to be bound */
static void _bboxes_draw_all()
{
    Vec2 win;
    unsigned int nbboxes;

    glUseProgram(bboxes_program);
    glUniformMatrix3fv(glGetUniformLocation(bboxes_program, "inverse_view_matrix"),
                       1, GL_FALSE,
                       (const GLfloat *) camera_get_inverse_view_matrix_ptr());
    win = game_get_window_size();
    glUniform1f(glGetUniformLocation(bboxes_program, "aspect"),
                win.x / win.y);
    glUniform1f(glGetUniformLocation(bboxes_program, "is_grid"), 0);

    glBindVertexArray(bboxes_vao);
    glBindBuffer(GL_ARRAY_BUFFER, bboxes_vbo);
    nbboxes = entitypool_size(bbox_pool);
    glBufferData(GL_ARRAY_BUFFER, nbboxes * sizeof(BBoxPoolElem),
                 entitypool_begin(bbox_pool), GL_STREAM_DRAW);
    glDrawArrays(GL_POINTS, 0, nbboxes);
}

/* --- grid ---------------------------------------------------------------- */

static Vec2 grid_size = { 1.0, 1.0 };

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

static Array *grid_cells; /* bboxes used for drawing grid */

static void _grid_create_cells()
{
    BBoxPoolElem *cell;
    BBox cbox, cellbox;
    Entity camera;
    Vec2 cur, skip;

    /* find camera bounds in world space */
    camera = camera_get();
    if (entity_eq(camera, entity_nil))
        cbox = bbox(vec2_zero, vec2(1, 1));
    else
        cbox = bbox_transform(transform_get_world_matrix(camera),
                                 bbox(vec2(-1, -1), vec2(1, 1)));

    /* find lower grid snap for min */
    if (grid_size.x > 0)
        cbox.min.x = grid_size.x * scalar_floor(cbox.min.x / grid_size.x);
    else
        cbox.min.x -= 0.5;
    if (grid_size.y > 0)
        cbox.min.y = grid_size.y * scalar_floor(cbox.min.y / grid_size.y);
    else
        cbox.min.y -= 0.5;

    /* create grid cell bbox */
    cellbox.min = vec2_zero;
    if (grid_size.x > 0)
        cellbox.max.x = grid_size.x;
    else
        cellbox.max.x = cbox.max.x - cbox.min.x + 1;
    if (grid_size.y > 0)
        cellbox.max.y = grid_size.y;
    else
        cellbox.max.y = cbox.max.y - cbox.min.y + 1;

    /* fill in with grid cells */
    for (cur.x = cbox.min.x; cur.x < cbox.max.x; cur.x += cellbox.max.x)
        for (cur.y = cbox.min.y; cur.y < cbox.max.y; cur.y += cellbox.max.y)
        {
            cell = array_add(grid_cells);
            cell->bbox = cellbox;
            cell->wmat = mat3_scaling_rotation_translation(vec2(1, 1), 0, cur);
            cell->selected = 0;
        }
}

static void _grid_init()
{
    grid_cells = array_new(BBoxPoolElem);
}
static void _grid_deinit()
{
    array_free(grid_cells);
}

static void _grid_draw()
{
    Vec2 win;
    unsigned int ncells;

    glUseProgram(bboxes_program);
    glUniformMatrix3fv(glGetUniformLocation(bboxes_program, "inverse_view_matrix"),
                       1, GL_FALSE,
                       (const GLfloat *) camera_get_inverse_view_matrix_ptr());
    win = game_get_window_size();
    glUniform1f(glGetUniformLocation(bboxes_program, "aspect"),
                win.x / win.y);
    glUniform1f(glGetUniformLocation(bboxes_program, "is_grid"), 1);

    _grid_create_cells();

    glBindVertexArray(bboxes_vao);
    glBindBuffer(GL_ARRAY_BUFFER, bboxes_vbo);
    ncells = array_length(grid_cells);
    glBufferData(GL_ARRAY_BUFFER, ncells * sizeof(BBoxPoolElem),
                 array_begin(grid_cells), GL_STREAM_DRAW);
    glDrawArrays(GL_POINTS, 0, ncells);
    array_clear(grid_cells);
}

/* ------------------------------------------------------------------------- */

void edit_init()
{
    uneditable_pool = entitypool_new(EntityPoolElem);

    _bboxes_init();
    _grid_init();
}
void edit_deinit()
{
    _grid_deinit();
    _bboxes_deinit();

    entitypool_free(uneditable_pool);
}

static void _uneditable_remove(Entity ent)
{
    entitypool_remove(uneditable_pool, ent);
}

void edit_update_all()
{
    entitypool_remove_destroyed(uneditable_pool, _uneditable_remove);

    _bboxes_update_all();
}

void edit_draw_all()
{
    if (!enabled)
        return;

    _bboxes_draw_all();
    _grid_draw();
}

void edit_save_all(Serializer *s)
{
    EntityPoolElem *elem;

    vec2_save(&grid_size, s);
    entitypool_save_foreach(elem, uneditable_pool, s);
}
void edit_load_all(Deserializer *s)
{
    EntityPoolElem *elem;

    vec2_load(&grid_size, s);
    entitypool_load_foreach(elem, uneditable_pool, s);
}
