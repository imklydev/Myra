#pragma once

#include <SDL3/SDL.h>

#include <string>

struct KeyboardState
{
    std::string text;
    bool enter = false;
    bool backspace = false;
    bool escape = false;
};

extern KeyboardState Keyboard;

void BeginKeyboardFrame();

void HandleKeyboard(SDL_Event *event);