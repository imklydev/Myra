#pragma once


#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "drawing.h"

// Rendering related stuff
extern SDL_Window *window;
extern SDL_Renderer *renderer;

void CreateSDLWindow(const char *title, int width, int height);

void InitRenderingBackend();

void QuitRenderingBackend();
