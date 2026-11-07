#include "../rendering/rendering.h"
#include "keyboard.h"
#include "mouse.h"

#include "screens/codeEditorScreen.h"
#include "screens/myraScreen.h"

int main(int argc, char *argv[])
{
    InitRenderingBackend();

    CreateSDLWindow("Myra", 800, 800);
    currentScreen = &MyraScreen;

    SDL_Event event;
    bool running = true;

    if (argc > 1)
    {
        SetCurrentScreen(&CodeEditorScreen);
        ActuallySetCurrentScreen();

        for (int i = 1; i < argc; i++)
        {
            CodeEditorScreen.OpenTab(argv[i]);
        }
    }

    // only show the Myra Screen if theres no folder/tabs open from previous sessions. (pretty bad implementation lol)
    if (!CodeEditorScreen.codeTabs[0].openedFile.empty())
    {
        SetCurrentScreen(&CodeEditorScreen);
        ActuallySetCurrentScreen();
    }

    while (running)
    {
        nextScreen = currentScreen;
        SDL_GetWindowSize(window, &ScreenWidth, &ScreenHeight);
        BeginKeyboardFrame();
        BeginMouseFrame();

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                if (currentScreen == &CodeEditorScreen)
                {
                    CodeEditorScreen.OnQuit();
                }
                running = false;
            }

            HandleKeyboard(&event);
            HandleMouse(&event);
        }

        SetDrawColor(BG_COLOR);
        SDL_RenderClear(renderer);

        currentScreen->update();
        currentScreen->draw();

        ActuallySetCurrentScreen();
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    QuitRenderingBackend();
    return 0;
}
