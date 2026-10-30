#pragma once

#include <SDL3/SDL.h>

#include <string>

struct KeyboardState
{
    std::string text;
    bool enter = false;
    bool backspace = false;
    bool escape = false;
    std::string move = "none";
};

extern KeyboardState Keyboard;

extern const bool *IsKeyPressed;

void BeginKeyboardFrame();

void HandleKeyboard(SDL_Event *event);