#include "rendering.h"

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
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
}

void QuitRenderingBackend()
{
    FreeAllText();
    FreeAllFonts();

    SDL_DestroyRenderer(renderer);

    SDL_StopTextInput(window);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();
}
