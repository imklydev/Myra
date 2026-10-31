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
    int CursorPos = -1; // = the character the cursor is selecting (-1 = begining, before any char)
    int SelectionStartPos = -1;
    int selectionLength;
    bool selectionMode = false;
    Vector2 offset = {0, 0}; // for scrolling

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
            CursorPos = GetLineEnd(CursorPos + 1);
            return;
        }

        size_t cursor = CursorPos + 1;

        int lastCursorColumn = cursor - GetLineStart(cursor);

        CursorPos = GetLineEnd(cursor) - 1;

        if (CursorPos < CodeSection.text.size() - 1)
        {
            MoveCursorRight();

            cursor = CursorPos + 1;

            int currentLineLength = GetLineEnd(cursor) - GetLineStart(cursor);

            if (currentLineLength == 0)
            {
                return;
            }

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

        DrawRect(Vector2(w + CodeSection.pos.x, line * lineHeight + offset.y), Vector2(2, h));
    }

    // Some AI generated code but I'm way too tired to actually rewrite this
    void DrawSelection()
    {
        const std::string &text = CodeSection.text;
        const size_t size = text.size();

        if (size == 0 || SelectionStartPos == CursorPos)
            return;

        TTF_Font *font = cachedFonts[FONT + "_" + std::to_string(16)];
        if (!font)
            return;

        const int lineHeight = TTF_GetFontLineSkip(font);

        // A cursor position p means "after character p", so the boundary index is p + 1.
        // Selection is the half-open range [selBegin, selEnd).
        long long a = static_cast<long long>(std::min(SelectionStartPos, CursorPos)) + 1;
        long long b = static_cast<long long>(std::max(SelectionStartPos, CursorPos)) + 1;

        a = std::clamp<long long>(a, 0, static_cast<long long>(size));
        b = std::clamp<long long>(b, 0, static_cast<long long>(size));

        const size_t selBegin = static_cast<size_t>(a);
        const size_t selEnd = static_cast<size_t>(b);

        if (selBegin >= selEnd)
            return;

        auto measure = [&](const std::string &s) -> int
        {
            if (s.empty())
                return 0;
            int w = 0, h = 0;
            TTF_GetStringSize(font, s.c_str(), 0, &w, &h);
            return w;
        };

        size_t lineStart = GetLineStart(selBegin);
        int line = static_cast<int>(std::count(text.begin(), text.begin() + lineStart, '\n'));

        // lineStart is always <= size here, so GetLineStart/GetLineEnd are safe.
        while (lineStart <= size)
        {
            const size_t lineEnd = GetLineEnd(lineStart); // index of '\n' or size

            const size_t segBegin = std::max(selBegin, lineStart);
            const size_t segEnd = std::min(selEnd, lineEnd);

            const float y = line * lineHeight + offset.y;

            if (segBegin < segEnd)
            {
                const std::string prefix = text.substr(lineStart, segBegin - lineStart);
                const std::string sel = text.substr(segBegin, segEnd - segBegin);

                const float x = CodeSection.pos.x + measure(prefix);
                const int selW = measure(sel);

                SetDrawColor(SELECTION_BG_COLOR);
                DrawRect(Vector2(x, y), Vector2(selW, lineHeight));

                SetDrawColor({0, 0, 0, 255});
                DrawText(sel, 16, Vector2(x, y));
                SetDrawColor(TEXT_COLOR);
            }

            // Selection continues past this line's '\n': stop here only if it doesn't.
            if (lineEnd >= selEnd)
                break;

            // Optional: small highlight so selected newlines (and empty lines) are visible.
            if (selBegin <= lineEnd)
            {
                const float x = CodeSection.pos.x + measure(text.substr(lineStart, lineEnd - lineStart));
                SetDrawColor(SELECTION_BG_COLOR);
                DrawRect(Vector2(x, y), Vector2(6, lineHeight));
                SetDrawColor(TEXT_COLOR);
            }

            // lineEnd < selEnd <= size, so text[lineEnd] == '\n' and lineEnd + 1 <= size.
            lineStart = lineEnd + 1;
            line++;
        }
    }

    void HandleBackspace()
    {
        if (selectionMode && CursorPos != SelectionStartPos)
        {

            CodeSection.text.erase(std::min(SelectionStartPos + 1, CursorPos + 1),
                                   selectionLength);

            if (CursorPos > SelectionStartPos)
            {
                CursorPos = SelectionStartPos;
            }

            selectionMode = false;
        }
        else
        {
            EraseCodePointAtCursorPos();
        }
    }

    void HandleDelete()
    {
        if (selectionMode && CursorPos != SelectionStartPos)
        {

            CodeSection.text.erase(std::min(SelectionStartPos + 1, CursorPos + 1),
                                   selectionLength);

            if (CursorPos > SelectionStartPos)
            {
                CursorPos = SelectionStartPos;
            }

            selectionMode = false;
        }
        else
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
    }

public:
    void update() override
    {
        if (!Keyboard.text.empty())
        {
            if (selectionMode)
            {
                HandleBackspace();
            }

            CodeSection.text.insert(CursorPos + 1, Keyboard.text);

            CursorPos += Keyboard.text.size();
        }

        // SHIFT select text
        if (!selectionMode && (IsKeyPressed[SDL_SCANCODE_LSHIFT] || IsKeyPressed[SDL_SCANCODE_RSHIFT]) && Keyboard.move != "none")
        {
            selectionMode = true;
            SelectionStartPos = CursorPos;
        }

        if ((!(Keyboard.text.empty())) ||
            (!(IsKeyPressed[SDL_SCANCODE_LSHIFT] || IsKeyPressed[SDL_SCANCODE_RSHIFT]) && Keyboard.move != "none"))
        {
            selectionMode = false;
        }

        if (selectionMode)
        {
            selectionLength = std::abs((SelectionStartPos + 1) - (CursorPos + 1));
            // std::cout << CodeSection.text.substr(std::min(SelectionStartPos + 1, CursorPos + 1),
            //                                      selectionLength);
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
            if (selectionMode)
            {
                HandleBackspace();
            }
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
        if ((IsKeyPressed[SDL_SCANCODE_LCTRL] || IsKeyPressed[SDL_SCANCODE_RCTRL]) && Keyboard.key == SDLK_O)
        {
            OpenFile();
        }

        // CTRL+S save file
        if ((IsKeyPressed[SDL_SCANCODE_LCTRL] || IsKeyPressed[SDL_SCANCODE_RCTRL]) && Keyboard.key == SDLK_S)
        {
            SaveFile();
        }

        // CTRL+V paste text
        if ((IsKeyPressed[SDL_SCANCODE_LCTRL] || IsKeyPressed[SDL_SCANCODE_RCTRL]) && Keyboard.key == SDLK_V)
        {
            char *pastedText = SDL_GetClipboardText();

            CodeSection.text.insert(CursorPos + 1, pastedText);

            CursorPos += strlen(pastedText);
        }

        // CTRL+C copy text
        if ((IsKeyPressed[SDL_SCANCODE_LCTRL] || IsKeyPressed[SDL_SCANCODE_RCTRL]) && Keyboard.key == SDLK_C)
        {
            std::string copiedText = CodeSection.text.substr(std::min(SelectionStartPos + 1, CursorPos + 1),
                                                             selectionLength);
            SDL_SetClipboardText(copiedText.c_str());
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
            HandleBackspace();
        }

        if (Keyboard.delete_pressed)
        {
            HandleDelete();
        }

        int lineCount = CalcCurrentLineCount();

        LineNumbersSection.setLineCount(lineCount);
    }

    void draw() override
    {
        LineNumbersSection.draw(offset);
        CodeSection.draw(offset);

        if (selectionMode && selectionLength > 0)
        {
            DrawSelection();
        }
        DrawCursor();
    }
};

codeEditorScreen CodeEditorScreen;
