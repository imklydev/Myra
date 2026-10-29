#include "../rendering/rendering.h"
#include "keyboard.h"

#include "screens/codeEditorScreen.h"

int main()
{
    InitRenderingBackend();

    CreateWindow("Myra", 800, 800);

    SDL_Event event;
    bool running = true;

    while (running)
    {
        BeginKeyboardFrame();

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }

            HandleKeyboard(&event);
        }

        if (Keyboard.escape)
        {
            running = false;
        }

        SetDrawColor(BG_COLOR);
        SDL_RenderClear(renderer);

        CodeEditorScreen.update();
        CodeEditorScreen.draw();

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    QuitRenderingBackend();
    return 0;
}
