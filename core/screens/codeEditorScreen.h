#pragma once

#include <fstream>
#include <iostream>
#include <string>

#include "../screen.h"
#include "../keyboard.h"

extern void ToggleShowProjectFolderOverview();
extern void OpenTab(char *file);
extern std::string GetOpenedFolder();

#include "../sections/lineNumbersSection.h"
#include "../sections/codeSection.h"
#include "../sections/tabSection.h"
#include "../sections/projectFolderOverview.h"

#include "../../third_party/tinyfiledialogs.h"

class codeEditorScreen : public Screen
{
private:
    int currentTab = 0;
    codeTab defaultTab = {{{"", -1}}, 0, "", "", -1, -1, 0, false, Vector2(0, 0)};
    std::vector<codeTab> codeTabs = {defaultTab};

    void EraseCodePointAtCursorPos()
    {
        if (codeTabs[currentTab].text.empty())
        {
            return;
        }

        if (codeTabs[currentTab].CursorPos < 0)
        {
            return;
        }

        size_t index = codeTabs[currentTab].CursorPos;

        size_t codePointSize = 1;

        while (index > 0 &&
               (static_cast<unsigned char>(codeTabs[currentTab].text[index]) & 0xC0) == 0x80)
        {
            --index;
            codePointSize += 1;
        }

        codeTabs[currentTab].text.erase(index, codePointSize);

        // Find the previous code point
        if (index == 0)
        {
            codeTabs[currentTab].CursorPos = -1;
        }
        else
        {
            codeTabs[currentTab].CursorPos--;

            while (codeTabs[currentTab].CursorPos > 0 &&
                   (static_cast<unsigned char>(codeTabs[currentTab].text[codeTabs[currentTab].CursorPos]) & 0xC0) == 0x80)
            {
                codeTabs[currentTab].CursorPos--;
            }
        }
    }

    void MoveCursorLeft()
    {
        if (codeTabs[currentTab].CursorPos < 0)
        {
            return;
        }

        codeTabs[currentTab].CursorPos--;

        while (codeTabs[currentTab].CursorPos > 0 &&
               (static_cast<unsigned char>(codeTabs[currentTab].text[codeTabs[currentTab].CursorPos]) & 0xC0) == 0x80)
        {
            codeTabs[currentTab].CursorPos--;
        }
    }

    void MoveCursorRight()
    {
        if (codeTabs[currentTab].CursorPos >= static_cast<int>(codeTabs[currentTab].text.size() - 1))
        {
            return;
        }

        codeTabs[currentTab].CursorPos++;

        while (codeTabs[currentTab].CursorPos < codeTabs[currentTab].text.size() &&
               (static_cast<unsigned char>(codeTabs[currentTab].text[codeTabs[currentTab].CursorPos]) & 0xC0) == 0x80)
        {
            codeTabs[currentTab].CursorPos++;
        }
    }

    void OpenFile()
    {
        char *path = tinyfd_openFileDialog(
            "Open file...",
            "../",
            0,
            nullptr,
            "text files",
            0);

        if (path == nullptr)
        {
            return;
        }

        codeTabs[currentTab].openedFile = path;

        if (!codeTabs[currentTab].openedFile.empty())
        {
            std::ifstream file(codeTabs[currentTab].openedFile);

            std::string line;
            codeTabs[currentTab].text = "";
            while (std::getline(file, line))
            {
                codeTabs[currentTab].text += line;
                codeTabs[currentTab].text += "\n";
            }

            file.close();
        }
    }

    void SaveFile()
    {
        if (codeTabs[currentTab].openedFile.empty())
        {
            char *path = tinyfd_saveFileDialog(
                "Save file...",
                "../",
                0,
                nullptr,
                nullptr);

            if (path == nullptr)
            {
                return;
            }

            codeTabs[currentTab].openedFile = path;
        }

        std::ofstream file(codeTabs[currentTab].openedFile);

        file << codeTabs[currentTab].text;

        file.close();
    }

    int CalcCurrentLineCount()
    {
        int lineCount = 1;
        for (char c : codeTabs[currentTab].text)
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

        while (pos > 0 && codeTabs[currentTab].text[pos - 1] != '\n')
            pos--;
        return pos;
    }

    size_t GetLineEnd(size_t pos) const
    {
        while (pos < codeTabs[currentTab].text.size() && codeTabs[currentTab].text[pos] != '\n')
            pos++;
        return pos;
    }

    void MoveCursorUp()
    {
        if (codeTabs[currentTab].CursorPos == -1)
        {
            return;
        }

        size_t cursor = codeTabs[currentTab].CursorPos + 1;

        int lastCursorColumn = cursor - GetLineStart(cursor);

        codeTabs[currentTab].CursorPos = GetLineStart(cursor) - 1;

        if (codeTabs[currentTab].CursorPos >= 0)
        {
            MoveCursorLeft();

            cursor = codeTabs[currentTab].CursorPos + 1;
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
        if (codeTabs[currentTab].CursorPos == -1)
        {
            codeTabs[currentTab].CursorPos = GetLineEnd(codeTabs[currentTab].CursorPos + 1);
            return;
        }

        size_t cursor = codeTabs[currentTab].CursorPos + 1;

        int lastCursorColumn = cursor - GetLineStart(cursor);

        codeTabs[currentTab].CursorPos = GetLineEnd(cursor) - 1;

        if (codeTabs[currentTab].CursorPos < codeTabs[currentTab].text.size() - 1)
        {
            MoveCursorRight();

            cursor = codeTabs[currentTab].CursorPos + 1;

            int currentLineLength = GetLineEnd(cursor) - GetLineStart(cursor);

            if (currentLineLength == 0)
            {
                return;
            }

            if (currentLineLength < lastCursorColumn)
            {
                codeTabs[currentTab].CursorPos = GetLineEnd(cursor) - 1;
                return;
            }

            int currentColumn = 0;
            while (currentColumn < lastCursorColumn)
            {
                currentColumn++;
                MoveCursorRight();
                cursor = codeTabs[currentTab].CursorPos + 1;
            }
        }
    }

    void DrawCursor()
    {
        TTF_Font *font = cachedFonts[FONT + "_" + std::to_string(16)];

        size_t lineStart = GetLineStart(codeTabs[currentTab].CursorPos + 1);

        int w, h;
        TTF_GetStringSize(font, codeTabs[currentTab].text.substr(lineStart, codeTabs[currentTab].CursorPos + 1 - lineStart).c_str(), 0, &w, &h);

        int lineHeight = TTF_GetFontLineSkip(font);

        int line = std::count(codeTabs[currentTab].text.begin(), codeTabs[currentTab].text.begin() + lineStart, '\n');

        DrawRect(Vector2(w + CodeSection.pos.x, line * lineHeight) + Vector2(0, TabSecton.size.y) + codeTabs[currentTab].offset + offsetCausedByProjectFolderOverview, Vector2(2, h));
    }

    void DrawSelection()
    {
        const std::string &text = codeTabs[currentTab].text;
        const size_t size = text.size();

        if (size == 0 || codeTabs[currentTab].SelectionStartPos == codeTabs[currentTab].CursorPos)
            return;

        TTF_Font *font = cachedFonts[FONT + "_" + std::to_string(16)];
        if (!font)
            return;

        const int lineHeight = TTF_GetFontLineSkip(font);

        // A cursor position p means "after character p", so the boundary index is p + 1.
        // Selection is the half-open range [selBegin, selEnd).
        long long a = static_cast<long long>(std::min(codeTabs[currentTab].SelectionStartPos, codeTabs[currentTab].CursorPos)) + 1;
        long long b = static_cast<long long>(std::max(codeTabs[currentTab].SelectionStartPos, codeTabs[currentTab].CursorPos)) + 1;

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

            const float y = line * lineHeight + codeTabs[currentTab].offset.y;

            if (segBegin < segEnd)
            {
                const std::string prefix = text.substr(lineStart, segBegin - lineStart);
                const std::string sel = text.substr(segBegin, segEnd - segBegin);

                const float x = CodeSection.pos.x + measure(prefix);
                const int selW = measure(sel);

                SetDrawColor(SELECTION_BG_COLOR);
                DrawRect(Vector2(x, y) + Vector2(0, TabSecton.size.y) + offsetCausedByProjectFolderOverview, Vector2(selW, lineHeight));

                SetDrawColor({0, 0, 0, 255});
                DrawText(sel, 16, Vector2(x, y) + Vector2(0, TabSecton.size.y) + offsetCausedByProjectFolderOverview);
                SetDrawColor(TEXT_COLOR);
            }

            // Selection continues past this line's '\n': stop here only if it doesn't.
            if (lineEnd >= selEnd)
                break;

            // small highlight so selected newlines (and empty lines) are visible.
            if (selBegin <= lineEnd)
            {
                const float x = CodeSection.pos.x + measure(text.substr(lineStart, lineEnd - lineStart));
                SetDrawColor(SELECTION_BG_COLOR);
                DrawRect(Vector2(x, y) + Vector2(0, TabSecton.size.y) + offsetCausedByProjectFolderOverview, Vector2(6, lineHeight));
                SetDrawColor(TEXT_COLOR);
            }

            // lineEnd < selEnd <= size, so text[lineEnd] == '\n' and lineEnd + 1 <= size.
            lineStart = lineEnd + 1;
            line++;
        }
    }

    void HandleBackspace()
    {
        if (codeTabs[currentTab].selectionMode && codeTabs[currentTab].CursorPos != codeTabs[currentTab].SelectionStartPos)
        {

            codeTabs[currentTab].text.erase(std::min(codeTabs[currentTab].SelectionStartPos + 1, codeTabs[currentTab].CursorPos + 1),
                                            codeTabs[currentTab].selectionLength);

            if (codeTabs[currentTab].CursorPos > codeTabs[currentTab].SelectionStartPos)
            {
                codeTabs[currentTab].CursorPos = codeTabs[currentTab].SelectionStartPos;
            }

            codeTabs[currentTab].selectionMode = false;
        }
        else
        {
            int cursor = codeTabs[currentTab].CursorPos;
            bool deletingATab = true;
            for (int i = 0; i < TAB_SIZE; i++)
            {
                if (cursor < 0)
                    break;

                if (codeTabs[currentTab].text[cursor] != ' ')
                {
                    deletingATab = false;
                    break;
                }
                cursor--;
            }

            if (deletingATab)
            {
                for (int i = 0; i < TAB_SIZE; i++)
                {
                    EraseCodePointAtCursorPos();
                }
            }
            else
            {
                EraseCodePointAtCursorPos();
            }
        }
    }

    void HandleDelete()
    {
        if (codeTabs[currentTab].selectionMode && codeTabs[currentTab].CursorPos != codeTabs[currentTab].SelectionStartPos)
        {

            codeTabs[currentTab].text.erase(std::min(codeTabs[currentTab].SelectionStartPos + 1, codeTabs[currentTab].CursorPos + 1),
                                            codeTabs[currentTab].selectionLength);

            if (codeTabs[currentTab].CursorPos > codeTabs[currentTab].SelectionStartPos)
            {
                codeTabs[currentTab].CursorPos = codeTabs[currentTab].SelectionStartPos;
            }

            codeTabs[currentTab].selectionMode = false;
        }
        else
        {
            int CursorPosSnapshot = codeTabs[currentTab].CursorPos;
            MoveCursorRight();

            if (CursorPosSnapshot == codeTabs[currentTab].CursorPos)
            {
                // the cursor is at the end of the text, so delete should do nothing
            }
            else
            {
                EraseCodePointAtCursorPos();
            }
        }
    }

    void PasteText()
    {
        char *pastedText = SDL_GetClipboardText();

        codeTabs[currentTab].text.insert(codeTabs[currentTab].CursorPos + 1, pastedText);

        codeTabs[currentTab].CursorPos += strlen(pastedText);
    }

    void CopyText()
    {
        std::string copiedText = codeTabs[currentTab].text.substr(std::min(codeTabs[currentTab].SelectionStartPos + 1, codeTabs[currentTab].CursorPos + 1),
                                                                  codeTabs[currentTab].selectionLength);
        SDL_SetClipboardText(copiedText.c_str());
    }

    void CutText()
    {
        std::string copiedText = codeTabs[currentTab].text.substr(std::min(codeTabs[currentTab].SelectionStartPos + 1, codeTabs[currentTab].CursorPos + 1),
                                                                  codeTabs[currentTab].selectionLength);
        SDL_SetClipboardText(copiedText.c_str());
        HandleBackspace();
    }

    void SelectAllText()
    {
        codeTabs[currentTab].selectionMode = true;
        codeTabs[currentTab].CursorPos = codeTabs[currentTab].text.size();
        codeTabs[currentTab].SelectionStartPos = -1;
    }

    void MoveToNextTab()
    {
        if (codeTabs.size() > currentTab + 1)
        {
            currentTab = currentTab + 1;
            MoveCursorLeft(); // Somehow even crappier fixto not make the cursor move when moving tabs
                              //, but fixes the screen jittering
        }
    }

    void MoveToPreviousTab()
    {
        if (currentTab > 0)
        {
            currentTab--;
            MoveCursorRight(); // Somehow even crappier fixto not make the cursor move when moving tabs
                               //, but fixes the screen jittering
        }
    }

    void NewTab()
    {
        codeTabs.push_back({{{"", -1}}, 0, "", "", -1, -1, 0, false, Vector2(0, 0)});
        currentTab = codeTabs.size() - 1;
    }

    void CloseCurrentTab()
    {
        if (codeTabs.size() > 1)
        {
            if (!codeTabs[currentTab].openedFile.empty())
            {
                SaveFile();
            }

            if (currentTab + 1 < codeTabs.size())
            {
                codeTabs.erase(codeTabs.begin() + currentTab);
            }
            else
            {
                codeTabs.erase(codeTabs.begin() + currentTab);
                currentTab--;
            }
        }
    }

    void SaveTextSnapshot()
    {
        codeTabs[currentTab].textSnapshots.push_back({codeTabs[currentTab].text, codeTabs[currentTab].CursorPos});
        codeTabs[currentTab].undoHistoryIndex = codeTabs[currentTab].textSnapshots.size() - 1; // ehh
    }

    void Undo()
    {
        if (codeTabs[currentTab].textSnapshots.size() <= 0 || codeTabs[currentTab].undoHistoryIndex < 0)
        {
            return;
        }

        codeTabs[currentTab].text = codeTabs[currentTab].textSnapshots[codeTabs[currentTab].undoHistoryIndex].text;
        codeTabs[currentTab].CursorPos = codeTabs[currentTab].textSnapshots[codeTabs[currentTab].undoHistoryIndex].CursorPos;

        codeTabs[currentTab].undoHistoryIndex--;
        std::cout << codeTabs[currentTab].undoHistoryIndex << " " << codeTabs[currentTab].textSnapshots.size() - 1 << "\n";
    }

    void Redo()
    {
        if (codeTabs[currentTab].undoHistoryIndex >= codeTabs[currentTab].textSnapshots.size() - 1)
        {
            return;
        }

        codeTabs[currentTab].undoHistoryIndex++;

        codeTabs[currentTab].text = codeTabs[currentTab].textSnapshots[codeTabs[currentTab].undoHistoryIndex].text;
        codeTabs[currentTab].CursorPos = codeTabs[currentTab].textSnapshots[codeTabs[currentTab].undoHistoryIndex].CursorPos;
    }

    void WriteTab()
    {
        std::string tabString = "";
        for (int i = 0; i < TAB_SIZE; i++)
        {
            tabString += ' ';
        }

        codeTabs[currentTab].text.insert(codeTabs[currentTab].CursorPos + 1, tabString);

        codeTabs[currentTab].CursorPos += tabString.size();
    }

    int GetLineStartSpacing(int pos)
    {
        if (pos == -1)
        {
            return 0;
        }
        int lineStart = GetLineStart(pos);
        int startSpacing = 0;

        while (codeTabs[currentTab].text[lineStart] == ' ')
        {
            startSpacing++;
            lineStart++;
        }

        return startSpacing;
    }

    void HandleEnter()
    {
        if (codeTabs[currentTab].selectionMode)
        {
            HandleBackspace();
        }
        std::string lineStartSpacing = std::string(GetLineStartSpacing((codeTabs[currentTab].CursorPos)), ' ');

        codeTabs[currentTab].text.insert(codeTabs[currentTab].CursorPos + 1, "\n");
        MoveCursorRight();

        codeTabs[currentTab].text.insert(codeTabs[currentTab].CursorPos + 1, lineStartSpacing);
        codeTabs[currentTab].CursorPos += lineStartSpacing.size();
    }

    void HandleSelectingText()
    {
        // SHIFT select text
        if (!codeTabs[currentTab].selectionMode && (IsKeyPressed[SDL_SCANCODE_LSHIFT] || IsKeyPressed[SDL_SCANCODE_RSHIFT]) && Keyboard.move != "none")
        {
            codeTabs[currentTab].selectionMode = true;
            codeTabs[currentTab].SelectionStartPos = codeTabs[currentTab].CursorPos;
        }

        if ((!(Keyboard.text.empty())) ||
            (!(IsKeyPressed[SDL_SCANCODE_LSHIFT] || IsKeyPressed[SDL_SCANCODE_RSHIFT]) && Keyboard.move != "none"))
        {
            codeTabs[currentTab].selectionMode = false;
        }

        if (codeTabs[currentTab].selectionMode)
        {
            codeTabs[currentTab].selectionLength = std::abs((codeTabs[currentTab].SelectionStartPos + 1) - (codeTabs[currentTab].CursorPos + 1));
        }
    }

public:
    bool shouldShowProjectFolderOverview = false;
    std::string openedFolder = "";

    void OpenTab(char *file)
    {
        if (codeTabs[0] == defaultTab)
        {
            codeTabs.erase(codeTabs.begin() + 0);
        }

        if (file == nullptr)
        {
            return;
        }

        codeTabs.push_back({{{"", -1}}, 0, "", file, -1, -1, 0, false, Vector2(0, 0)});
        currentTab = codeTabs.size() - 1;

        if (!codeTabs[currentTab].openedFile.empty())
        {
            std::ifstream file(codeTabs[currentTab].openedFile);

            std::string line;
            codeTabs[currentTab].text = "";
            while (std::getline(file, line))
            {
                codeTabs[currentTab].text += line;
                codeTabs[currentTab].text += "\n";
            }

            file.close();
        }
    }

    void OpenFolder()
    {
        char *path = tinyfd_selectFolderDialog(
            "Open folder...",
            "../");

        if (path == nullptr)
        {
            return;
        }

        openedFolder = path;
    }

    void update() override
    {
        TabSecton.codeTabs = &codeTabs;

        if (!Keyboard.text.empty())
        {
            if (codeTabs[currentTab].selectionMode)
            {
                HandleBackspace();
            }

            // for now, save text to previousText for ctrl-z if space is pressed
            if (Keyboard.text.find(' ') != std::string::npos)
            {
                SaveTextSnapshot();
            }

            codeTabs[currentTab].text.insert(codeTabs[currentTab].CursorPos + 1, Keyboard.text);
            codeTabs[currentTab].CursorPos += Keyboard.text.size();

            // Some standard simple autocompletion quality of life features
            if (Keyboard.text == "{")
            {
                codeTabs[currentTab].text.insert(codeTabs[currentTab].CursorPos + 1, "}");
            }
            else if (Keyboard.text == "(")
            {
                codeTabs[currentTab].text.insert(codeTabs[currentTab].CursorPos + 1, ")");
            }
        }

        // CTRL+SHIFT+O open folder
        if ((IsKeyPressed[SDL_SCANCODE_LCTRL] || IsKeyPressed[SDL_SCANCODE_RCTRL]) &&
            (IsKeyPressed[SDL_SCANCODE_LSHIFT] || IsKeyPressed[SDL_SCANCODE_RSHIFT]) &&
            Keyboard.key == SDLK_O)
        {
            OpenFolder();
        }
        else // to make sure these 2 don't run at the same time
        {
            // CTRL+O open file
            if ((IsKeyPressed[SDL_SCANCODE_LCTRL] || IsKeyPressed[SDL_SCANCODE_RCTRL]) && Keyboard.key == SDLK_O)
            {
                OpenFile();
            }
        }

        // CTRL+S save file
        if ((IsKeyPressed[SDL_SCANCODE_LCTRL] || IsKeyPressed[SDL_SCANCODE_RCTRL]) && Keyboard.key == SDLK_S)
        {
            SaveFile();
        }

        // CTRL+V paste text
        if ((IsKeyPressed[SDL_SCANCODE_LCTRL] || IsKeyPressed[SDL_SCANCODE_RCTRL]) && Keyboard.key == SDLK_V)
        {
            SaveTextSnapshot();
            PasteText();
        }

        // CTRL+C copy text
        if ((IsKeyPressed[SDL_SCANCODE_LCTRL] || IsKeyPressed[SDL_SCANCODE_RCTRL]) && Keyboard.key == SDLK_C)
        {
            CopyText();
        }

        // CTRL+X cut text
        if ((IsKeyPressed[SDL_SCANCODE_LCTRL] || IsKeyPressed[SDL_SCANCODE_RCTRL]) && Keyboard.key == SDLK_X)
        {
            SaveTextSnapshot();
            CutText();
        }

        // CTRL+A cut text
        if ((IsKeyPressed[SDL_SCANCODE_LCTRL] || IsKeyPressed[SDL_SCANCODE_RCTRL]) && Keyboard.key == SDLK_A)
        {
            SelectAllText();
        }

        // CTRL+ALT+RIGHT_ARROW move to right tab
        if ((IsKeyPressed[SDL_SCANCODE_LCTRL] || IsKeyPressed[SDL_SCANCODE_RCTRL]) &&
            (IsKeyPressed[SDL_SCANCODE_LALT] || IsKeyPressed[SDL_SCANCODE_RALT]) &&
            Keyboard.key == SDLK_RIGHT)
        {
            MoveToNextTab();
        }

        // CTRL+ALT+LEFT_ARROW move to left tab
        if ((IsKeyPressed[SDL_SCANCODE_LCTRL] || IsKeyPressed[SDL_SCANCODE_RCTRL]) &&
            (IsKeyPressed[SDL_SCANCODE_LALT] || IsKeyPressed[SDL_SCANCODE_RALT]) &&
            Keyboard.key == SDLK_LEFT)
        {
            MoveToPreviousTab();
        }

        // CTRL+N new tab/file
        if ((IsKeyPressed[SDL_SCANCODE_LCTRL] || IsKeyPressed[SDL_SCANCODE_RCTRL]) &&
            Keyboard.key == SDLK_N)
        {
            NewTab();
        }

        // CTRL+Q close current tab (and save too)
        if ((IsKeyPressed[SDL_SCANCODE_LCTRL] || IsKeyPressed[SDL_SCANCODE_RCTRL]) &&
            Keyboard.key == SDLK_Q)
        {
            CloseCurrentTab();
        }

        // CTRL+Z undo
        if ((IsKeyPressed[SDL_SCANCODE_LCTRL] || IsKeyPressed[SDL_SCANCODE_RCTRL]) &&
            Keyboard.key == SDLK_Z)
        {
            Undo();
        }

        // CTRL+Y undo
        if ((IsKeyPressed[SDL_SCANCODE_LCTRL] || IsKeyPressed[SDL_SCANCODE_RCTRL]) &&
            Keyboard.key == SDLK_Y)
        {
            Redo();
        }

        // SHIFT select text
        HandleSelectingText();

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
            SaveTextSnapshot();
            HandleEnter();
        }

        if (Keyboard.tab)
        {
            WriteTab();
        }

        // CTRL+BACKSPACE deletes the whole word
        if ((IsKeyPressed[SDL_SCANCODE_LCTRL] || IsKeyPressed[SDL_SCANCODE_RCTRL]) &&
            Keyboard.backspace)
        {
            HandleBackspace();
            int cursor = codeTabs[currentTab].CursorPos;
            while (cursor >= 0 && codeTabs[currentTab].text[cursor] != ' ' && codeTabs[currentTab].text[cursor] != '\n')
            {
                HandleBackspace();
                cursor--;
            }
        }
        else
        {
            if (Keyboard.backspace)
            {
                HandleBackspace();
            }
        }

        if (Keyboard.delete_pressed)
        {
            HandleDelete();
        }

        if (isMouseHoveringOver(CodeSection.pos + codeTabs[currentTab].offset + Vector2(0, TabSecton.size.y) + offsetCausedByProjectFolderOverview, CodeSection.size))
        {
            if (Mouse.scroll)
            {
                codeTabs[currentTab].offset.y += Mouse.scroll * SCROLL_SENSITIVITY;

                if (codeTabs[currentTab].offset.y > 0)
                {
                    codeTabs[currentTab].offset.y = 0;
                }
            }
        }

        int lineCount = CalcCurrentLineCount();
        LineNumbersSection.setLineCount(lineCount);

        CodeSection.text = codeTabs[currentTab].text;
    }

    Vector2 offsetCausedByProjectFolderOverview = {0, 0};
    void draw() override
    {
        if (shouldShowProjectFolderOverview)
        {
            ProjectFolderOverview.draw({0, 0});
            offsetCausedByProjectFolderOverview = Vector2(ProjectFolderOverview.size.x, 0);
        }
        else
        {
            offsetCausedByProjectFolderOverview = {0, 0};
        }

        LineNumbersSection.draw(codeTabs[currentTab].offset + Vector2(0, TabSecton.size.y) + offsetCausedByProjectFolderOverview);
        CodeSection.draw(codeTabs[currentTab].offset + Vector2(0, TabSecton.size.y) + offsetCausedByProjectFolderOverview);

        if (codeTabs[currentTab].selectionMode && codeTabs[currentTab].selectionLength > 0)
        {
            DrawSelection();
        }
        DrawCursor();

        TabSecton.draw(Vector2{0, 0}, currentTab);
    }
};

inline codeEditorScreen CodeEditorScreen;
