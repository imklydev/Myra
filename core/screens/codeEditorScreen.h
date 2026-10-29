#pragma once

#include <fstream>
#include <string>

#include "../screen.h"
#include "../keyboard.h"

#include "../sections/lineNumbersSection.h"
#include "../sections/codeSection.h"

#include "../../third_party/tinyfiledialogs.h"

class codeEditorScreen : public Screen
{
private:
    const char *openedFile = nullptr;

    void EraseLastCodePoint(std::string &text)
    {
        if (text.empty())
        {
            return;
        }

        size_t index = text.size() - 1;

        size_t codePointSize = 1;

        while (index > 0 &&
               (static_cast<unsigned char>(text[index]) & 0xC0) == 0x80)
        {
            --index;
            codePointSize += 1;
        }

        text.erase(index, codePointSize);
    }

public:
    void update() override
    {
        if (!Keyboard.text.empty())
        {
            CodeSection.text += Keyboard.text;
        }

        if (Keyboard.enter)
        {
            CodeSection.text += '\n';
        }

        // CTRL+O open file
        if ((IsKeyPressed[SDL_SCANCODE_LCTRL] || IsKeyPressed[SDL_SCANCODE_RCTRL]) && IsKeyPressed[SDL_SCANCODE_O])
        {
            openedFile = tinyfd_openFileDialog(
                "Open file...",
                "../",
                0,
                nullptr,
                "text files",
                0);
            
            if(openedFile != nullptr)
            {
                std::ifstream file(openedFile);

                std::string line;
                CodeSection.text = "";
                while (std::getline(file, line))
                {
                    CodeSection.text += line;
                    CodeSection.text += "\n";
                }

                file.close();
            }

        }

        // CTRL+S save file
        if ((IsKeyPressed[SDL_SCANCODE_LCTRL] || IsKeyPressed[SDL_SCANCODE_RCTRL]) && IsKeyPressed[SDL_SCANCODE_S])
        {
            if (openedFile == nullptr)
            {
                openedFile = tinyfd_saveFileDialog(
                    "Save file...",
                    "../",
                    0,
                    nullptr,
                    "text files");
            }

            std::ofstream file(openedFile);

            file << CodeSection.text;

            file.close();
        }

        if (Keyboard.backspace && !CodeSection.text.empty())
        {
            EraseLastCodePoint(CodeSection.text);
        }

        int lineCount = 1;
        for (char c : CodeSection.text)
        {
            if (c == '\n')
            {
                ++lineCount;
            }
        }

        LineNumbersSection.setLineCount(lineCount);
    }

    void draw() override
    {
        LineNumbersSection.draw();
        CodeSection.draw();
    }
};

codeEditorScreen CodeEditorScreen;
