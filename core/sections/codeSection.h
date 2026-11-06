#pragma once

#include "../section.h"

inline std::unordered_map<std::string, SDL_Color> C_style_coloring_keywords = {
    // variable types
    {"int", SDL_Color{65, 65, 120, 255}},
    {"string", SDL_Color{45, 100, 45, 255}},
    {"char", SDL_Color{204, 153, 255, 255}},
    {"unsigned", SDL_Color{75, 75, 165, 255}},
    {"long", SDL_Color{0, 102, 153, 255}},
    {"float", SDL_Color{204, 153, 0, 255}},

    // declaration keywords
    {"struct", SDL_Color{169, 65, 217, 255}},
    {"class", SDL_Color{217, 214, 109, 255}},
    {"namespace", SDL_Color{65, 217, 182, 255}},

    // operators
    {"=", SDL_Color{235, 235, 195, 255}},
    {"+", SDL_Color{235, 235, 195, 255}},
    {"-", SDL_Color{235, 235, 195, 255}},
    {"/", SDL_Color{235, 235, 195, 255}},
    {"*", SDL_Color{235, 235, 195, 255}},

    // statement keywords
    {"if", SDL_Color{184, 44, 209, 255}},
    {"while", SDL_Color{184, 44, 209, 255}},
    {"for", SDL_Color{184, 44, 209, 255}},
    {"do", SDL_Color{184, 44, 209, 255}},

    // including keywords (these namaes keep getting dumber and dumber lol)
    {"#include", SDL_Color{103, 28, 188, 255}},
    {"using", SDL_Color{211, 172, 30, 255}},
};

class codeSection : public Section
{
private:
    std::vector<TextSpan> GetTextColoringSpans()
    {
        std::vector<TextSpan> textSpans;
        for (size_t i = 0; i < text.size(); i++)
        {

            // for string coloring
            if (text[i] == '"' || text[i] == '\'' || text[i] == '`')
            {
                char end_char = text[i];
                size_t start = i;
                i++;

                while (i < text.size() && text[i] != end_char)
                {
                    i++;
                }

                textSpans.push_back({start, i + 1, SDL_Color{255, 204, 102, 255}});
                continue;
            }

            // for comment coloring
            if (i + 1 < text.size() && text[i] == '/' && text[i + 1] == '/')
            {
                size_t start = i;

                while (i < text.size() && text[i] != '\n')
                {
                    i++;
                }

                textSpans.push_back({start, i + 1, SDL_Color{62, 134, 78, 255}});
                continue;
            }

            // for number coloring
            if (text[i] >= '0' && text[i] <= '9')
            {
                TextSpan span;
                size_t start = i;

                while (text[i] >= '0' && text[i] <= '9')
                {
                    i++;
                }

                span.start = start;
                span.end = i;
                span.color = LIGHT_GREEN;
                textSpans.push_back(span);
                continue;
            }

            for (auto keyword : C_style_coloring_keywords)
            {
                if (i + keyword.first.size() > text.size())
                {
                    continue;
                }

                if (text.substr(i, keyword.first.size()) == keyword.first)
                {

                    if (i + keyword.first.size() < text.size())
                    {
                        // to prevenet highlighting keywords that are in variable names and such, skip highliting them if
                        // the next character after the keywords is a letter or underscore
                        char nextChar = text[i + keyword.first.size()];
                        if ((nextChar >= 'A' && nextChar <= 'Z') ||
                            (nextChar >= 'a' && nextChar <= 'z') ||
                            (nextChar == '_'))
                        {
                            continue;
                        }
                    }

                    textSpans.push_back({i, i + keyword.first.size(), keyword.second});
                    i += keyword.first.size();
                }
            }
        }

        return textSpans;
    }

public:
    Vector2 pos = {60, 0};
    Vector2 size = {4000, 40000};
    float textSize = 16;
    std::string text = "";
    std::vector<TextSpan> TextSpans = {};

    void RecalculateTextSpans()
    {
        TextSpans = GetTextColoringSpans();
    }

    void draw(Vector2 pos_offset) override
    {
        SetDrawColor(BG_COLOR);
        DrawRect(pos + pos_offset, size);

        SetDrawColor(TEXT_COLOR);
        DrawText(text, textSize, pos + pos_offset, size, TextSpans);
    }
};

inline codeSection CodeSection;
