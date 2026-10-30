#include "keyboard.h"

KeyboardState Keyboard;
const bool *IsKeyPressed;

void BeginKeyboardFrame()
{
    Keyboard.text.clear();
    Keyboard.enter = false;
    Keyboard.backspace = false;
    Keyboard.escape = false;
    Keyboard.move = "none";

    IsKeyPressed = SDL_GetKeyboardState(nullptr);
}

void HandleKeyboard(SDL_Event *event)
{

    if (event->type == SDL_EVENT_TEXT_INPUT)
    {
        for (const char *c = event->text.text; *c; ++c) // iterate through all chars of the SDL text input
        {
            if (*c != '\r' && *c != '\n')
            {
                Keyboard.text += *c;
            }
        }
    }
    else if (event->type == SDL_EVENT_KEY_DOWN)
    {
        switch (event->key.key)
        {
        case SDLK_RETURN:
        case SDLK_KP_ENTER: // (keypad enter)
            Keyboard.enter = true;
            break;
        case SDLK_BACKSPACE:
            Keyboard.backspace = true;
            break;
        case SDLK_ESCAPE:
            Keyboard.escape = true;
            break;
        case SDLK_LEFT:
            Keyboard.move = "left";
            break;
        case SDLK_RIGHT:
            Keyboard.move = "right";
            break;
        case SDLK_UP:
            Keyboard.move = "up";
            break;
        case SDLK_DOWN:
            Keyboard.move = "down";
            break;
        default:
            break;
        }
    }
}
