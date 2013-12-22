#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <Freeimage.h>

#include "sprite.h"
#include "transform.h"

SDL_Window *sdl_window;
SDL_GLContext sdl_context;

static void game_init()
{
    /* initialize SDL, force core profile */
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
            SDL_GL_CONTEXT_PROFILE_CORE);

    /* create window and GL context */
    sdl_window = SDL_CreateWindow("cgame", 100, 100, 800, 600,
            SDL_WINDOW_OPENGL);
    sdl_context = SDL_GL_CreateContext(sdl_window);

    /* initialize GLEW */
    glewExperimental = GL_TRUE;
    glewInit();
    glGetError(); /* see http://www.opengl.org/wiki/OpenGL_Loading_Library */

    /* some GL settings */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    /* init systems */
    sprite_init();

    /* add test object */

    Entity ent = entity_gen();

    transform_add(ent);
    transform_set_origin(ent, vec2(0.0f, 0.0f));

    sprite_add(ent);
    sprite_set_cell(ent, vec2( 0.0f, 32.0f));
    sprite_set_size(ent, vec2(32.0f, 32.0f));
}

static void game_deinit()
{
    /* deinit systems */
    sprite_deinit();

    /* deinit SDL */
    SDL_GL_DeleteContext(sdl_context);
    SDL_Quit();
}

static bool game_events()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
        switch (event.type)
        {
            case SDL_QUIT:
                return false;

            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    return false;
                break;

            default:
                break;
        }

    return true;
}

static void game_update(float dt)
{
    /* test update */
    Vec2 v = transform_get_origin(0);
    v.x += 2 * dt;
    transform_set_origin(0, v);
    
    /* update systems */
    sprite_update_all();
}

static void game_draw()
{
    glClearColor(1.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    /* draw systems */
    sprite_draw_all();

    SDL_GL_SwapWindow(sdl_window);
}

void game_run()
{
    game_init();

    while (game_events())
    {
        game_update(0.1);
        game_draw();
    }

    game_deinit();
}

