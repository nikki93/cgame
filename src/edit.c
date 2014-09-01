#include "edit.h"

#include <stdio.h>

#include "error.h"
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

bool edit_bboxes_has(Entity ent)
{
    return entitypool_get(bbox_pool, ent) != NULL;
}
BBox edit_bboxes_get(Entity ent)
{
    BBoxPoolElem *elem = entitypool_get(bbox_pool, ent);
    error_assert(elem);
    return elem->bbox;
}

unsigned int edit_bboxes_get_num()
{
    return entitypool_size(bbox_pool);
}
EntityBBoxPair edit_bboxes_get_nth(unsigned int n)
{
    BBoxPoolElem *elem;
    EntityBBoxPair pair;

    error_assert(n < entitypool_size(bbox_pool));
    elem = entitypool_nth(bbox_pool, n);

    pair.ent = elem->pool_elem.ent;
    pair.bbox = elem->bbox;
    return pair;
}

void edit_bboxes_set_selected(Entity ent, bool selected)
{
    BBoxPoolElem *elem = entitypool_get(bbox_pool, ent);
    error_assert(elem);
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
    static BBox defaultbb = { { -0.25, -0.25 }, { 0.25, 0.25 } };

    if (!enabled)
        return;

    entitypool_foreach(elem, bbox_pool)
    {
        ent = elem->pool_elem.ent;
        if (!transform_has(ent))
            continue;

        /* update world matrix */
        elem->wmat = transform_get_world_matrix(ent);

        /* if no bbox, make default */
        if (elem->bbox.max.x - elem->bbox.min.x <= SCALAR_EPSILON
            || elem->bbox.max.y - elem->bbox.min.y <= SCALAR_EPSILON)
            elem->bbox = defaultbb;
    }
}

/* needs bbox shader program to be bound */
static void _bboxes_draw_all()
{
    Vec2 win;
    unsigned int nbboxes;

    glUseProgram(bboxes_program);
    glUniformMatrix3fv(glGetUniformLocation(bboxes_program,
                                            "inverse_view_matrix"),
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
    Vec2 cur, csize;

    /* find camera bounds in world space */
    camera = camera_get_current_camera();
    if (entity_eq(camera, entity_nil))
        cbox = bbox(vec2(-1, -1), vec2(1, 1));
    else
        cbox = bbox_transform(transform_get_world_matrix(camera),
                                 bbox(vec2(-1, -1), vec2(1, 1)));
    csize = vec2(cbox.max.x - cbox.min.x, cbox.max.y - cbox.min.y);

    /* create grid cell bbox */
    cellbox.min = vec2_zero;
    if (grid_size.x > 0)
        cellbox.max.x = grid_size.x;
    else
        cellbox.max.x = csize.x + 1;
    if (grid_size.y > 0)
        cellbox.max.y = grid_size.y;
    else
        cellbox.max.y = csize.y + 1;

    /* make it bigger if it's too small */
    while (csize.x / cellbox.max.x > 70 || csize.y / cellbox.max.y > 70)
        cellbox.max = vec2_scalar_mul(cellbox.max, 2);

    /* find lower grid snap for min */
    if (grid_size.x > 0)
        cbox.min.x = cellbox.max.x * scalar_floor(cbox.min.x / cellbox.max.x);
    else
        cbox.min.x -= 0.5;
    if (grid_size.y > 0)
        cbox.min.y = cellbox.max.y * scalar_floor(cbox.min.y / cellbox.max.y);
    else
        cbox.min.y -= 0.5;

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
    glUniformMatrix3fv(glGetUniformLocation(bboxes_program,
                                            "inverse_view_matrix"),
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

/* --- line ---------------------------------------------------------------- */

static GLuint line_program;
static GLuint line_vao;
static GLuint line_vbo;

typedef struct LinePoint LinePoint;
struct LinePoint
{
    Vec2 position;
    Scalar point_size;
    Color color;
};

static Array *line_points; /* each consecutive pair is a line */

void edit_line_add(Vec2 a, Vec2 b, Scalar point_size, Color color)
{
    LinePoint *lp;

    lp = array_add(line_points);
    lp->position = a;
    lp->point_size = point_size;
    lp->color = color;

    lp = array_add(line_points);
    lp->position = b;
    lp->point_size = point_size;
    lp->color = color;
}

static void _line_init()
{
    line_points = array_new(LinePoint);

    /* init draw stuff */
    line_program = gfx_create_program(data_path("edit_line.vert"),
                                      NULL,
                                      data_path("edit_line.frag"));
    glUseProgram(line_program);
    glGenVertexArrays(1, &line_vao);
    glBindVertexArray(line_vao);
    glGenBuffers(1, &line_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, line_vbo);
    gfx_bind_vertex_attrib(line_program, GL_FLOAT, 2, "position", LinePoint,
                           position);
    gfx_bind_vertex_attrib(line_program, GL_FLOAT, 1, "point_size", LinePoint,
                           point_size);
    gfx_bind_vertex_attrib(line_program, GL_FLOAT, 4, "color", LinePoint,
                           color);
}
static void _line_deinit()
{
    /* clean up draw stuff */
    glDeleteProgram(line_program);
    glDeleteBuffers(1, &line_vbo);
    glDeleteVertexArrays(1, &line_vao);

    array_free(line_points);
}

static void _line_draw_all()
{
    unsigned int npoints;

    /* bind program, update uniforms */
    glUseProgram(line_program);
    glUniformMatrix3fv(glGetUniformLocation(line_program,
                                            "inverse_view_matrix"),
                       1, GL_FALSE,
                       (const GLfloat *) camera_get_inverse_view_matrix_ptr());

    /* draw! */
    glBindVertexArray(line_vao);
    glBindBuffer(GL_ARRAY_BUFFER, line_vao);
    npoints = array_length(line_points);
    glBufferData(GL_ARRAY_BUFFER, npoints * sizeof(LinePoint),
                 array_begin(line_points), GL_STREAM_DRAW);
    glDrawArrays(GL_LINES, 0, npoints);
    glDrawArrays(GL_POINTS, 0, npoints);
}

/* ------------------------------------------------------------------------- */

void edit_clear()
{
    entitypool_clear(bbox_pool);
    array_clear(line_points);
}

void edit_init()
{
    uneditable_pool = entitypool_new(EntityPoolElem);

    _bboxes_init();
    _grid_init();
    _line_init();
}
void edit_deinit()
{
    _line_deinit();
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
    _line_draw_all();
}

void edit_save_all(Store *s)
{
    Store *t, *elem_s;
    EntityPoolElem *elem;

    if (store_child_save(&t, "edit", s))
    {
        vec2_save(&grid_size, "grid_size", t);
        entitypool_save_foreach(elem, elem_s, uneditable_pool,
                                "uneditable_pool", t);
    }
}
void edit_load_all(Store *s)
{
    Store *t, *elem_s;
    EntityPoolElem *elem;

    if (store_child_load(&t, "edit", s))
    {
        vec2_load(&grid_size, "grid_size", grid_size, t);
        entitypool_load_foreach(elem, elem_s, uneditable_pool,
                                "uneditable_pool", t);
    }
}
