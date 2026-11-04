#include <iostream>
#include "rendering.h"

// Some usefull stuff
int ScreenWidth = 800;
int ScreenHeight = 800;

// Rendering related stuff
SDL_Window *window;
SDL_Renderer *renderer;

void CreateSDLWindow(const char *title, int width, int height)
{
    window = SDL_CreateWindow(title, width, height, SDL_WINDOW_RESIZABLE);

    if (!window)
    {
        return;
    }

    renderer = SDL_CreateRenderer(window, nullptr);

    if (!renderer)
    {
        return;
    }

    SDL_SetRenderVSync(renderer, 1);

    SDL_StartTextInput(window);
}

void InitRenderingBackend()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "SDL3 (the rendering backend) couldn't start.";
        exit(0);
    }

    if (!TTF_Init())
    {
        std::cout << "SDL3_TTF (used for fonts) couldn't start.";
        exit(0);
    }
    
}

void QuitRenderingBackend()
{
    FreeAllText();
    FreeAllFonts();
    FreeAllImages();

    SDL_DestroyRenderer(renderer);

    SDL_StopTextInput(window);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();
}
