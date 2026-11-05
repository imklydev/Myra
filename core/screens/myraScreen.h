#pragma once

#include <fstream>
#include <iostream>
#include <string>

#include "../screen.h"
#include "../keyboard.h"

#include "codeEditorScreen.h"

class myraScreen : public Screen
{
private:
    Vector2i myraTextSize = {0, 0};
    int fontSize = 128;

public:
    bool isFirstFrame = true;

    void update() override
    {
        if (isFirstFrame)
        {
            isFirstFrame = false;
            TTF_GetStringSize(GetFont(fontSize), "Myra", 0, &myraTextSize.x, &myraTextSize.y);
        }

        if (Keyboard.key)
        {
            SetCurrentScreen(&CodeEditorScreen);
        }
    }

    void draw() override
    {
        SetDrawColor(TEXT_COLOR);
        DrawText("Myra", fontSize, Vector2(ScreenWidth / 2 - myraTextSize.x / 2, 100));

        DrawText("Thank you for trying the Myra code editor!\nYou can press any key to exit this menu.", 16, Vector2(ScreenWidth / 2 - 200, 300));

        DrawText("Some usefull keybinds, while you're here:\nCTRL+N - New tab\nCTRL+Q - Close tab\nCTRL+ALT+LEFT - Move to previous tab\nCTRL+ALT+RIGHT - Move to next tab\nCTRL+O - Open file\nCTRL+SHIFT+O - Open folder", 16, Vector2(ScreenWidth / 2 - 200, 500));
    }
};

myraScreen MyraScreen;
