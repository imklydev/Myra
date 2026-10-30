#pragma once

#include <fstream>
#include <iostream>
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
    int CursorPos = -1; // = the character the cursor is selecting
    Vector2 offset = {0, 0};

    void EraseCodePointAtCursorPos()
    {
        if (CodeSection.text.empty())
        {
            return;
        }

        if (CursorPos < 0)
        {
            return;
        }

        size_t index = CursorPos;

        size_t codePointSize = 1;

        while (index > 0 &&
               (static_cast<unsigned char>(CodeSection.text[index]) & 0xC0) == 0x80)
        {
            --index;
            codePointSize += 1;
        }

        CodeSection.text.erase(index, codePointSize);

        // Find the previous code point
        if (index == 0)
        {
            CursorPos = -1;
        }
        else
        {
            CursorPos--;

            while (CursorPos > 0 &&
                   (static_cast<unsigned char>(CodeSection.text[CursorPos]) & 0xC0) == 0x80)
            {
                CursorPos--;
            }
        }
    }

    void MoveCursorLeft()
    {
        if (CursorPos < 0)
        {
            return;
        }

        CursorPos--;

        while (CursorPos > 0 &&
               (static_cast<unsigned char>(CodeSection.text[CursorPos]) & 0xC0) == 0x80)
        {
            CursorPos--;
        }
    }

    void MoveCursorRight()
    {
        if (CursorPos >= static_cast<int>(CodeSection.text.size() - 1))
        {
            return;
        }

        CursorPos++;

        while (CursorPos < CodeSection.text.size() &&
               (static_cast<unsigned char>(CodeSection.text[CursorPos]) & 0xC0) == 0x80)
        {
            CursorPos++;
        }
    }

    void OpenFile()
    {
        openedFile = tinyfd_openFileDialog(
            "Open file...",
            "../",
            0,
            nullptr,
            "text files",
            0);

        if (openedFile != nullptr)
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

    void SaveFile()
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

    int CalcCurrentLineCount()
    {
        int lineCount = 1;
        for (char c : CodeSection.text)
        {
            if (c == '\n')
            {
                lineCount++;
            }
        }
        return lineCount;
    }

    size_t GetLineStart(size_t pos) const
    {
        while (pos > 0 && CodeSection.text[pos - 1] != '\n')
            pos--;
        return pos;
    }

    size_t GetLineEnd(size_t pos) const
    {
        while (pos < CodeSection.text.size() && CodeSection.text[pos] != '\n')
            pos++;
        return pos;
    }

    void MoveCursorUp()
    {
        if (CursorPos == -1)
        {
            return;
        }

        size_t cursor = CursorPos + 1;

        int lastCursorColumn = cursor - GetLineStart(cursor);

        CursorPos = GetLineStart(cursor) - 1;

        if (CursorPos >= 0)
        {
            MoveCursorLeft();

            cursor = CursorPos + 1;
            int currentColumn = cursor - GetLineStart(cursor);
            while (currentColumn > lastCursorColumn)
            {
                cursor--;
                currentColumn--;
                MoveCursorLeft();
            }
        }
    }

    void MoveCursorDown()
    {
        if (CursorPos == -1)
        {
            CursorPos = 0;
        }

        size_t cursor = CursorPos + 1;

        int lastCursorColumn = cursor - GetLineStart(cursor);

        CursorPos = GetLineEnd(cursor) - 1;

        if (CursorPos < CodeSection.text.size() - 1)
        {
            MoveCursorRight();

            cursor = CursorPos + 1;

            int currentLineLength = GetLineEnd(cursor) - GetLineStart(cursor) + 1;
            if (currentLineLength < lastCursorColumn)
            {
                CursorPos = GetLineEnd(cursor) - 1;
                return;
            }

            int currentColumn = 0;
            while (currentColumn < lastCursorColumn)
            {
                currentColumn++;
                MoveCursorRight();
                cursor = CursorPos + 1;
            }
        }
    }

    void DrawCursor()
    {
        TTF_Font *font = cachedFonts[FONT + "_" + std::to_string(16)];

        size_t lineStart = GetLineStart(CursorPos + 1);

        int w, h;
        TTF_GetStringSize(font, CodeSection.text.substr(lineStart, CursorPos + 1 - lineStart).c_str(), 0, &w, &h);

        int lineHeight = TTF_GetFontLineSkip(font);

        int line = std::count(CodeSection.text.begin(), CodeSection.text.begin() + lineStart, '\n');

        DrawRect(Vector2(w + 60, line * lineHeight + offset.y), Vector2(2, h));
    }

public:
    void update() override
    {
        if (!Keyboard.text.empty())
        {
            CodeSection.text.insert(CursorPos + 1, Keyboard.text);

            CursorPos += Keyboard.text.size();
        }

        // Cursor movement
        if (Keyboard.move == "left")
        {
            MoveCursorLeft();
        }
        else if (Keyboard.move == "right")
        {
            MoveCursorRight();
        }
        else if (Keyboard.move == "up")
        {
            MoveCursorUp();
        }
        else if (Keyboard.move == "down")
        {
            MoveCursorDown();
        }

        if (Keyboard.enter)
        {
            CodeSection.text.insert(CursorPos + 1, "\n");
            MoveCursorRight();
        }

        if (Keyboard.tab)
        {
            std::string tabString = "";
            for (int i = 0; i < TAB_SIZE; i++)
            {
                tabString += ' ';
            }

            CodeSection.text.insert(CursorPos + 1, tabString);

            CursorPos += tabString.size();
        }

        // CTRL+O open file
        if ((IsKeyPressed[SDL_SCANCODE_LCTRL] || IsKeyPressed[SDL_SCANCODE_RCTRL]) && IsKeyPressed[SDL_SCANCODE_O])
        {
            OpenFile();
        }

        // CTRL+S save file
        if ((IsKeyPressed[SDL_SCANCODE_LCTRL] || IsKeyPressed[SDL_SCANCODE_RCTRL]) && IsKeyPressed[SDL_SCANCODE_S])
        {
            SaveFile();
        }

        if (Mouse.scroll)
        {
            offset.y += Mouse.scroll * SCROLL_SENSITIVITY;

            if (offset.y > 0)
            {
                offset.y = 0;
            }
        }

        if (Keyboard.backspace)
        {
            EraseCodePointAtCursorPos();
        }

        if (Keyboard.delete_pressed)
        {
            int CursorPosSnapshot = CursorPos;
            MoveCursorRight();

            if (CursorPosSnapshot == CursorPos)
            {
                // the cursor is at the end of the text, so delete should do nothing
            }
            else
            {
                EraseCodePointAtCursorPos();
            }
        }

        int lineCount = CalcCurrentLineCount();

        LineNumbersSection.setLineCount(lineCount);
    }

    void draw() override
    {
        LineNumbersSection.draw(offset);
        CodeSection.draw(offset);
        DrawCursor();
    }
};

codeEditorScreen CodeEditorScreen;
