#include "sprite.h"

#include <string.h>
#include <stdlib.h>
#include <GL/glew.h>

#include "error.h"
#include "entitypool.h"
#include "dirs.h"
#include "mat3.h"
#include "saveload.h"
#include "transform.h"
#include "gfx.h"
#include "camera.h"
#include "texture.h"
#include "edit.h"

typedef struct Sprite Sprite;
struct Sprite
{
    EntityPoolElem pool_elem;

    Mat3 wmat; /* world transform matrix to send to shader */

    Vec2 size;
    Vec2 texcell;
    Vec2 texsize;

    int depth;
};

static EntityPool *pool;

static char *atlas = NULL;

/* GL stuff */
static GLuint program;
static GLuint vao;
static GLuint vbo;

/* ------------------------------------------------------------------------- */

static void _update_atlas()
{
    Vec2 atlas_size;

    texture_load(atlas);

    atlas_size = texture_get_size(atlas);
    glUseProgram(program);
    glUniform2fv(glGetUniformLocation(program, "atlas_size"), 1,
                 (const GLfloat *) &atlas_size);
}
void sprite_set_atlas(const char *filename)
{
    free(atlas);
    atlas = malloc(strlen(filename) + 1);
    strcpy(atlas, filename);
    _update_atlas();
}
const char *sprite_get_atlas()
{
    return atlas;
}

void sprite_add(Entity ent)
{
    Sprite *sprite;

    if (entitypool_get(pool, ent))
        return; /* already has a sprite */

    transform_add(ent);

    sprite = entitypool_add(pool, ent);
    sprite->size = vec2(1.0f, 1.0f);
    sprite->texcell = vec2(32.0f, 32.0f);
    sprite->texsize = vec2(32.0f, 32.0f);
    sprite->depth = 0;
}
void sprite_remove(Entity ent)
{
    entitypool_remove(pool, ent);
}
bool sprite_has(Entity ent)
{
    return entitypool_get(pool, ent) != NULL;
}

void sprite_set_size(Entity ent, Vec2 size)
{
    Sprite *sprite = entitypool_get(pool, ent);
    error_assert(sprite);
    sprite->size = size;
}
Vec2 sprite_get_size(Entity ent)
{
    Sprite *sprite = entitypool_get(pool, ent);
    error_assert(sprite);
    return sprite->size;
}

void sprite_set_texcell(Entity ent, Vec2 texcell)
{
    Sprite *sprite = entitypool_get(pool, ent);
    error_assert(sprite);
    sprite->texcell = texcell;
}
Vec2 sprite_get_texcell(Entity ent)
{
    Sprite *sprite = entitypool_get(pool, ent);
    error_assert(sprite);
    return sprite->texcell;
}
void sprite_set_texsize(Entity ent, Vec2 texsize)
{
    Sprite *sprite = entitypool_get(pool, ent);
    error_assert(sprite);
    sprite->texsize = texsize;
}
Vec2 sprite_get_texsize(Entity ent)
{
    Sprite *sprite = entitypool_get(pool, ent);
    error_assert(sprite);
    return sprite->texsize;
}

void sprite_set_depth(Entity ent, int depth)
{
    Sprite *sprite = entitypool_get(pool, ent);
    error_assert(sprite);
    sprite->depth = depth;
}
int sprite_get_depth(Entity ent)
{
    Sprite *sprite = entitypool_get(pool, ent);
    error_assert(sprite);
    return sprite->depth;
}

/* ------------------------------------------------------------------------- */

void sprite_init()
{
    /* initialize pool */
    pool = entitypool_new(Sprite);

    /* create shader program, load atlas */
    program = gfx_create_program(data_path("sprite.vert"),
                                 data_path("sprite.geom"),
                                 data_path("sprite.frag"));
    glUseProgram(program);
    glUniform1i(glGetUniformLocation(program, "tex0"), 0);
    sprite_set_atlas(data_path("default.png"));

    /* make vao, vbo, bind attributes */
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    gfx_bind_vertex_attrib(program, GL_FLOAT, 3, "wmat1", Sprite, wmat.m[0]);
    gfx_bind_vertex_attrib(program, GL_FLOAT, 3, "wmat2", Sprite, wmat.m[1]);
    gfx_bind_vertex_attrib(program, GL_FLOAT, 3, "wmat3", Sprite, wmat.m[2]);
    gfx_bind_vertex_attrib(program, GL_FLOAT, 2, "size", Sprite, size);
    gfx_bind_vertex_attrib(program, GL_FLOAT, 2, "texcell", Sprite, texcell);
    gfx_bind_vertex_attrib(program, GL_FLOAT, 2, "texsize", Sprite, texsize);
}

void sprite_deinit()
{
    /* clean up GL stuff */
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    /* deinit pool */
    entitypool_free(pool);

    free(atlas);
}

void sprite_update_all()
{
    Sprite *sprite;
    static Vec2 min = { -0.5, -0.5 }, max = { 0.5, 0.5 };

    entitypool_remove_destroyed(pool, sprite_remove);

    /* update world transform matrices */
    entitypool_foreach(sprite, pool)
        sprite->wmat = transform_get_world_matrix(sprite->pool_elem.ent);

    /* update edit bbox */
    if (edit_get_enabled())
        entitypool_foreach(sprite, pool)
            edit_bboxes_update(sprite->pool_elem.ent,
                               bbox(vec2_mul(sprite->size, min),
                                    vec2_mul(sprite->size, max)));
}

static int _depth_compare(const void *a, const void *b)
{
    const Sprite *sa = a, *sb = b;

    /* descending, break ties by Entity id for stability */
    if (sb->depth == sa->depth)
        return ((int) sa->pool_elem.ent.id) - ((int) sb->pool_elem.ent.id);
    return sb->depth - sa->depth;
}

void sprite_draw_all()
{
    unsigned int nsprites;

    /* depth sort */
    entitypool_sort(pool, _depth_compare);

    /* bind program, update uniforms */
    glUseProgram(program);
    glUniformMatrix3fv(glGetUniformLocation(program, "inverse_view_matrix"),
                       1, GL_FALSE,
                       (const GLfloat *) camera_get_inverse_view_matrix_ptr());

    /* bind atlas */
    glActiveTexture(GL_TEXTURE0);
    texture_bind(atlas);

    /* draw! */
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    nsprites = entitypool_size(pool);
    glBufferData(GL_ARRAY_BUFFER, nsprites * sizeof(Sprite),
                 entitypool_begin(pool), GL_STREAM_DRAW);
    glDrawArrays(GL_POINTS, 0, nsprites);
}

void sprite_save_all(Store *s)
{
    Store *t, *sprite_s;
    Sprite *sprite;

    if (store_child_save(&t, "sprite", s))
    {
        string_save((const char **) &atlas, "atlas", t);

        entitypool_save_foreach(sprite, sprite_s, pool, "pool", t)
        {
            vec2_save(&sprite->size, "size", sprite_s);
            vec2_save(&sprite->texcell, "texcell", sprite_s);
            vec2_save(&sprite->texsize, "texsize", sprite_s);
            int_save(&sprite->depth, "depth", sprite_s);
        }
    }
}
void sprite_load_all(Store *s)
{
    Store *t, *sprite_s;
    Sprite *sprite;
    char *tatlas;

    if (store_child_load(&t, "sprite", s))
    {
        /* only replace atlas if actually found something */
        if (string_load(&tatlas, "atlas", NULL, t))
        {
            free(atlas);
            atlas = tatlas;
            _update_atlas();
        }

        entitypool_load_foreach(sprite, sprite_s, pool, "pool", t)
        {
            vec2_load(&sprite->size, "size", vec2(1, 1), sprite_s);
            vec2_load(&sprite->texcell, "texcell", vec2(32, 32), sprite_s);
            vec2_load(&sprite->texsize, "texsize", vec2(32, 32), sprite_s);
            int_load(&sprite->depth, "depth", 0, sprite_s);
        }
    }
}

