#include <SDL.h>
#include <SDL_opengl.h>
#include <vector>
#include <conio.h>
#include <stdio.h>

bool try_sdl(int major, int minor, bool core = true, int option_tier = 0)
{
    printf(
        "Trying GL context %d.%d (%s profile, attribute tier %d)\n",
        major, minor, core ? "core" : "compatibility", option_tier
    );

    if(SDL_Init(SDL_INIT_EVERYTHING))
    {
        printf("SDL_Init: %s\n", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK,
        core ? SDL_GL_CONTEXT_PROFILE_CORE : SDL_GL_CONTEXT_PROFILE_COMPATIBILITY
    );
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);

    if(option_tier >= 1)
    {
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    }
    if(option_tier >= 2)
    {
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    }
    if(option_tier >= 3)
    {
        SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, false);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
    }
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_Window* win = SDL_CreateWindow(
        "Ayy lmao",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1280,
        720,
        SDL_WINDOW_OPENGL
    );
    SDL_GLContext ctx;

    if(!win)
    {
        printf("Couldn't even open a goddamn window: %s\n", SDL_GetError());
        goto fail_window;
    }

    ctx = SDL_GL_CreateContext(win);
    if(!ctx)
    {
        printf("Couldn't open the context: %s\n", SDL_GetError());
        goto fail_context;
    }

    SDL_GL_DeleteContext(ctx);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return true;
fail_context:
    SDL_DestroyWindow(win);
fail_window:
    SDL_Quit();
    return false;
}

struct config
{
    int major, minor;
    bool core;
    int option_tier;
};

void gen_configs(std::vector<config>& configs, int major, int minor)
{
    for(int i = 0; i < 4; ++i)
    {
        configs.push_back({major, minor, false, i});
        configs.push_back({major, minor, true, i});
    }
}

int main(int argc, char** argv)
{
    std::vector<config> configs;

    gen_configs(configs, 3, 0);
    gen_configs(configs, 3, 1);
    gen_configs(configs, 3, 2);
    gen_configs(configs, 3, 3);
    gen_configs(configs, 4, 0);
    gen_configs(configs, 4, 1);
    gen_configs(configs, 4, 2);
    gen_configs(configs, 4, 3);
    gen_configs(configs, 4, 4);
    gen_configs(configs, 4, 5);
    gen_configs(configs, 4, 6);

    for(config c : configs)
    {
        if (try_sdl(c.major, c.minor, c.core, c.option_tier)) printf("\t Success!\n");
        else printf("\t Failure!\n");
    }

    printf("Please abuse your keyboard to close this\n");
    _getch();
    return 0;
}