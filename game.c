#include <stdlib.h>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <Freeimage.h>

#include "sprite.h"
#include "transform.h"

static SDL_Window *sdl_window;
static SDL_GLContext sdl_context;

static Entity player;

static void _test_init()
{
    Entity block;
    unsigned int i;

    player = entity_new();

    transform_add(player);
    transform_set_origin(player, vec2(0.0f, 0.0f));

    sprite_add(player);
    sprite_set_cell(player, vec2( 0.0f, 32.0f));
    sprite_set_size(player, vec2(32.0f, 32.0f));

    for (i = 0; i < 100; ++i)
    {
        block = entity_new();

        transform_add(block);
        transform_set_origin(block,
                vec2((rand() % 25) - 12, (rand() % 9) - 4));

        sprite_add(block);
        sprite_set_cell(block, vec2(32.0f, 32.0f));
        sprite_set_size(block, vec2(32.0f, 32.0f));
    }
}

static void _test_update()
{
}

static void _game_init()
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

    /* test */
    _test_init();
}

static void _game_deinit()
{
    /* deinit systems */
    sprite_deinit();

    /* deinit SDL */
    SDL_GL_DeleteContext(sdl_context);
    SDL_Quit();
}

static bool _game_events()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
        switch (event.type)
        {
            case SDL_QUIT:
                return false;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_UP:
                        entity_new_message(player, MSG_DESTROY, 0);
                        break;

                    default:
                        break;
                }
                break;

            case SDL_KEYUP:
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        return false;

                    default:
                        break;
                }
                break;

            default:
                break;
        }

    return true;
}

static void _game_update(float dt)
{
    /* test update */
    Vec2 v = transform_get_origin(player);
    v.x += 0.5 * dt;
    transform_set_origin(player, v);
    
    /* check messages */
    sprite_check_messages_all();
    
    /* update systems */
    sprite_update_all();
    entity_update_all();

    /* debug */
    /*Vec2 player_pos = transform_get_origin(player);*/
    /*Vec2 block_pos = transform_get_origin(block);*/
    /*printf("(%f, %f), (%f, %f)\n", player_pos.x, player_pos.y,*/
            /*block_pos.x, block_pos.y);*/
}

static void _game_draw()
{
    glClearColor(1.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    /* draw systems */
    sprite_draw_all();

    SDL_GL_SwapWindow(sdl_window);
}

void game_run()
{
    _game_init();

    while (_game_events())
    {
        _game_update(0.1);
        _game_draw();
    }

    _game_deinit();
}

