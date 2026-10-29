#pragma once

#include "../screen.h"
#include "../keyboard.h"

#include "../sections/lineNumbersSection.h"
#include "../sections/codeSection.h"

class codeEditorScreen : public Screen
{
private:
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
