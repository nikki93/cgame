#include "edit.h"

#include "gfx.h"
#include "entitypool.h"
#include "mat3.h"
#include "transform.h"
#include "camera.h"
#include "dirs.h"

static bool enabled = false;

typedef struct BBoxPoolElem BBoxPoolElem;
struct BBoxPoolElem
{
    EntityPoolElem pool_elem;

    Mat3 wmat;
    BBox bbox;
    Scalar selected; /* >= 0.5 if and only if selected */
};

static EntityPool *bbox_pool;

/* ------------------------------------------------------------------------- */

void edit_set_enabled(bool e)
{
    enabled = e;
}
bool edit_get_enabled()
{
    return enabled;
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

/* ------------------------------------------------------------------------- */

static GLuint program;
static GLuint vao;
static GLuint vbo;

void edit_init()
{
    /* init pool */
    bbox_pool = entitypool_new(BBoxPoolElem);

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
    gfx_bind_vertex_attrib(program, GL_FLOAT, 2, "selected",
                           BBoxPoolElem, selected);
}
void edit_deinit()
{
    /* clean up GL stuff */
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    /* deinit pool */
    entitypool_free(bbox_pool);
}

#include "input.h"

void edit_update_all()
{
    BBoxPoolElem *elem;

    /* testing: delete on right click */
    Entity ent;
    Mat3 t;
    Vec2 m, p;
    if (input_mouse_down(MC_RIGHT))
    {
        m = camera_unit_to_world(input_get_mouse_pos_unit());
        
        entitypool_foreach(elem, bbox_pool)
        {
            ent = elem->pool_elem.ent;
            t = transform_get_world_matrix(ent);
            t = mat3_inverse(t);

            p = mat3_transform(t, m);
            if (bbox_contains(elem->bbox, p))
                entity_destroy(ent);
        }
    }

    /* update bbox world matrices */
    entitypool_foreach(elem, bbox_pool)
        elem->wmat = transform_get_world_matrix(elem->pool_elem.ent);
}

void edit_draw_all()
{
    unsigned int nbboxes;

    if (!enabled)
        return;

    /* bind program, update uniforms */
    glUseProgram(program);
    glUniformMatrix3fv(glGetUniformLocation(program, "inverse_view_matrix"),
                       1, GL_FALSE,
                       (const GLfloat *) camera_get_inverse_view_matrix_ptr());

    /* draw! */
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    nbboxes = entitypool_size(bbox_pool);
    glBufferData(GL_ARRAY_BUFFER, nbboxes * sizeof(BBoxPoolElem),
                 entitypool_begin(bbox_pool), GL_STREAM_DRAW);
    glDrawArrays(GL_POINTS, 0, nbboxes);
}
