#pragma once

#include "../section.h"
#include "../screens/codeEditorScreen.h"

inline std::unordered_map<std::string, SDL_Color> Cpp_style_coloring_keywords = {
    // variable types
    {"int", SDL_Color{65, 65, 120, 255}},
    {"string", SDL_Color{45, 100, 45, 255}},
    {"char", SDL_Color{204, 153, 255, 255}},
    {"unsigned", SDL_Color{75, 75, 165, 255}},
    {"long", SDL_Color{0, 102, 153, 255}},
    {"float", SDL_Color{204, 153, 0, 255}},
    {"bool", SDL_Color{132, 52, 181, 255}},
    {"const", SDL_Color{142, 136, 28, 255}},
    {"void", SDL_Color{132, 52, 181, 255}},

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
    {"switch", SDL_Color{184, 44, 209, 255}},
    {"case", SDL_Color{184, 44, 209, 255}},
    {"default", SDL_Color{184, 44, 209, 255}},
    {"else", SDL_Color{184, 44, 209, 255}},
    {"while", SDL_Color{184, 44, 209, 255}},
    {"for", SDL_Color{184, 44, 209, 255}},
    {"do", SDL_Color{184, 44, 209, 255}},
    {"break", SDL_Color{103, 28, 188, 255}},
    {"continue", SDL_Color{103, 28, 188, 255}},

    // including keywords (these namaes keep getting dumber and dumber lol)
    {"#include", SDL_Color{103, 28, 188, 255}},
    {"using", SDL_Color{211, 172, 30, 255}},

    // boolean values
    {"true", SDL_Color{27, 188, 102, 255}},
    {"false", SDL_Color{174, 39, 39, 255}},

    // default values
    {"null", SDL_Color{103, 28, 188, 255}},
    {"nullptr", SDL_Color{103, 28, 188, 255}},
};

inline std::unordered_map<std::string, SDL_Color> Python_style_coloring_keywords = {
    // built-in types / type constructors
    {"int", SDL_Color{65, 65, 120, 255}},
    {"str", SDL_Color{45, 100, 45, 255}},
    {"float", SDL_Color{204, 153, 0, 255}},
    {"bool", SDL_Color{132, 52, 181, 255}},
    {"list", SDL_Color{75, 75, 165, 255}},
    {"dict", SDL_Color{0, 102, 153, 255}},
    {"tuple", SDL_Color{204, 153, 255, 255}},
    {"set", SDL_Color{142, 136, 28, 255}},
    {"bytes", SDL_Color{204, 153, 255, 255}},
    {"complex", SDL_Color{204, 153, 0, 255}},

    // declaration keywords
    {"def", SDL_Color{169, 65, 217, 255}},
    {"class", SDL_Color{217, 214, 109, 255}},
    {"lambda", SDL_Color{169, 65, 217, 255}},
    {"global", SDL_Color{65, 217, 182, 255}},
    {"nonlocal", SDL_Color{65, 217, 182, 255}},

    // operators
    {"=", SDL_Color{235, 235, 195, 255}},
    {"+", SDL_Color{235, 235, 195, 255}},
    {"-", SDL_Color{235, 235, 195, 255}},
    {"/", SDL_Color{235, 235, 195, 255}},
    {"*", SDL_Color{235, 235, 195, 255}},
    {"%", SDL_Color{235, 235, 195, 255}},
    {"//", SDL_Color{235, 235, 195, 255}},
    {"**", SDL_Color{235, 235, 195, 255}},

    // logical operators (words in Python)
    {"and", SDL_Color{235, 235, 195, 255}},
    {"or", SDL_Color{235, 235, 195, 255}},
    {"not", SDL_Color{235, 235, 195, 255}},
    {"in", SDL_Color{235, 235, 195, 255}},
    {"is", SDL_Color{235, 235, 195, 255}},

    // statement keywords
    {"if", SDL_Color{184, 44, 209, 255}},
    {"elif", SDL_Color{184, 44, 209, 255}},
    {"else", SDL_Color{184, 44, 209, 255}},
    {"match", SDL_Color{184, 44, 209, 255}},
    {"case", SDL_Color{184, 44, 209, 255}},
    {"while", SDL_Color{184, 44, 209, 255}},
    {"for", SDL_Color{184, 44, 209, 255}},
    {"break", SDL_Color{103, 28, 188, 255}},
    {"continue", SDL_Color{103, 28, 188, 255}},
    {"pass", SDL_Color{103, 28, 188, 255}},
    {"return", SDL_Color{103, 28, 188, 255}},
    {"yield", SDL_Color{103, 28, 188, 255}},

    // exception handling
    {"try", SDL_Color{184, 44, 209, 255}},
    {"except", SDL_Color{184, 44, 209, 255}},
    {"finally", SDL_Color{184, 44, 209, 255}},
    {"raise", SDL_Color{103, 28, 188, 255}},
    {"assert", SDL_Color{103, 28, 188, 255}},

    // async
    {"async", SDL_Color{169, 65, 217, 255}},
    {"await", SDL_Color{169, 65, 217, 255}},

    // context / scoping
    {"with", SDL_Color{184, 44, 209, 255}},
    {"as", SDL_Color{211, 172, 30, 255}},
    {"del", SDL_Color{103, 28, 188, 255}},

    // importing keywords
    {"import", SDL_Color{103, 28, 188, 255}},
    {"from", SDL_Color{211, 172, 30, 255}},

    // special values
    {"True", SDL_Color{27, 188, 102, 255}},
    {"False", SDL_Color{174, 39, 39, 255}},
    {"None", SDL_Color{150, 150, 150, 255}},

    // self / cls
    {"self", SDL_Color{217, 130, 80, 255}},
    {"cls", SDL_Color{217, 130, 80, 255}},

    // common built-in functions
    {"print", SDL_Color{65, 150, 217, 255}},
    {"len", SDL_Color{65, 150, 217, 255}},
    {"range", SDL_Color{65, 150, 217, 255}},
    {"type", SDL_Color{65, 150, 217, 255}},
    {"isinstance", SDL_Color{65, 150, 217, 255}},
    {"enumerate", SDL_Color{65, 150, 217, 255}},
    {"zip", SDL_Color{65, 150, 217, 255}},
    {"map", SDL_Color{65, 150, 217, 255}},
    {"filter", SDL_Color{65, 150, 217, 255}},
    {"open", SDL_Color{65, 150, 217, 255}},
    {"input", SDL_Color{65, 150, 217, 255}},
    {"super", SDL_Color{65, 150, 217, 255}},
};

inline std::unordered_map<std::string, SDL_Color> JavaScript_and_TypeScript_style_coloring_keywords = {
    // TypeScript additions
    {"interface", SDL_Color{217, 214, 109, 255}},
    {"type", SDL_Color{169, 65, 217, 255}},
    {"enum", SDL_Color{169, 65, 217, 255}},
    {"namespace", SDL_Color{65, 217, 182, 255}},
    {"declare", SDL_Color{169, 65, 217, 255}},
    {"abstract", SDL_Color{169, 65, 217, 255}},
    {"implements", SDL_Color{217, 214, 109, 255}},
    {"readonly", SDL_Color{142, 136, 28, 255}},
    {"public", SDL_Color{169, 65, 217, 255}},
    {"private", SDL_Color{169, 65, 217, 255}},
    {"protected", SDL_Color{169, 65, 217, 255}},
    {"keyof", SDL_Color{235, 235, 195, 255}},
    {"infer", SDL_Color{235, 235, 195, 255}},
    {"satisfies", SDL_Color{235, 235, 195, 255}},

    // TypeScript primitive types
    {"string", SDL_Color{45, 100, 45, 255}},
    {"number", SDL_Color{204, 153, 0, 255}},
    {"boolean", SDL_Color{132, 52, 181, 255}},
    {"any", SDL_Color{174, 39, 39, 255}},
    {"unknown", SDL_Color{174, 39, 39, 255}},
    {"never", SDL_Color{174, 39, 39, 255}},
    {"object", SDL_Color{0, 102, 153, 255}},
    {"symbol", SDL_Color{204, 153, 255, 255}},
    {"bigint", SDL_Color{204, 153, 0, 255}},

    // declaration keywords (variables)
    {"var", SDL_Color{65, 65, 120, 255}},
    {"let", SDL_Color{75, 75, 165, 255}},
    {"const", SDL_Color{142, 136, 28, 255}},

    // built-in types / constructors
    {"String", SDL_Color{45, 100, 45, 255}},
    {"Number", SDL_Color{204, 153, 0, 255}},
    {"Boolean", SDL_Color{132, 52, 181, 255}},
    {"Object", SDL_Color{0, 102, 153, 255}},
    {"Array", SDL_Color{75, 75, 165, 255}},
    {"Symbol", SDL_Color{204, 153, 255, 255}},
    {"BigInt", SDL_Color{204, 153, 0, 255}},
    {"Map", SDL_Color{0, 102, 153, 255}},
    {"Set", SDL_Color{142, 136, 28, 255}},
    {"Promise", SDL_Color{65, 217, 182, 255}},
    {"Date", SDL_Color{204, 153, 255, 255}},
    {"JSON", SDL_Color{65, 150, 217, 255}},
    {"Math", SDL_Color{65, 150, 217, 255}},

    // declaration keywords
    {"function", SDL_Color{169, 65, 217, 255}},
    {"class", SDL_Color{217, 214, 109, 255}},
    {"extends", SDL_Color{217, 214, 109, 255}},
    {"static", SDL_Color{169, 65, 217, 255}},
    {"get", SDL_Color{169, 65, 217, 255}},
    {"set", SDL_Color{169, 65, 217, 255}},
    {"new", SDL_Color{169, 65, 217, 255}},

    // operators
    {"=", SDL_Color{235, 235, 195, 255}},
    {"+", SDL_Color{235, 235, 195, 255}},
    {"-", SDL_Color{235, 235, 195, 255}},
    {"/", SDL_Color{235, 235, 195, 255}},
    {"*", SDL_Color{235, 235, 195, 255}},
    {"%", SDL_Color{235, 235, 195, 255}},
    {"**", SDL_Color{235, 235, 195, 255}},
    {"=>", SDL_Color{235, 235, 195, 255}},
    {"===", SDL_Color{235, 235, 195, 255}},
    {"!==", SDL_Color{235, 235, 195, 255}},
    {"==", SDL_Color{235, 235, 195, 255}},
    {"!=", SDL_Color{235, 235, 195, 255}},
    {"&&", SDL_Color{235, 235, 195, 255}},
    {"||", SDL_Color{235, 235, 195, 255}},
    {"??", SDL_Color{235, 235, 195, 255}},
    {"!", SDL_Color{235, 235, 195, 255}},

    // word operators
    {"typeof", SDL_Color{235, 235, 195, 255}},
    {"instanceof", SDL_Color{235, 235, 195, 255}},
    {"in", SDL_Color{235, 235, 195, 255}},
    {"of", SDL_Color{235, 235, 195, 255}},
    {"delete", SDL_Color{235, 235, 195, 255}},
    {"void", SDL_Color{132, 52, 181, 255}},

    // statement keywords
    {"if", SDL_Color{184, 44, 209, 255}},
    {"else", SDL_Color{184, 44, 209, 255}},
    {"switch", SDL_Color{184, 44, 209, 255}},
    {"case", SDL_Color{184, 44, 209, 255}},
    {"default", SDL_Color{184, 44, 209, 255}},
    {"while", SDL_Color{184, 44, 209, 255}},
    {"for", SDL_Color{184, 44, 209, 255}},
    {"do", SDL_Color{184, 44, 209, 255}},
    {"break", SDL_Color{103, 28, 188, 255}},
    {"continue", SDL_Color{103, 28, 188, 255}},
    {"return", SDL_Color{103, 28, 188, 255}},
    {"yield", SDL_Color{103, 28, 188, 255}},

    // exception handling
    {"try", SDL_Color{184, 44, 209, 255}},
    {"catch", SDL_Color{184, 44, 209, 255}},
    {"finally", SDL_Color{184, 44, 209, 255}},
    {"throw", SDL_Color{103, 28, 188, 255}},

    // async
    {"async", SDL_Color{169, 65, 217, 255}},
    {"await", SDL_Color{169, 65, 217, 255}},

    // module keywords
    {"import", SDL_Color{103, 28, 188, 255}},
    {"export", SDL_Color{103, 28, 188, 255}},
    {"from", SDL_Color{211, 172, 30, 255}},
    {"as", SDL_Color{211, 172, 30, 255}},
    {"require", SDL_Color{211, 172, 30, 255}},

    // special values
    {"true", SDL_Color{27, 188, 102, 255}},
    {"false", SDL_Color{174, 39, 39, 255}},
    {"null", SDL_Color{150, 150, 150, 255}},
    {"undefined", SDL_Color{150, 150, 150, 255}},
    {"NaN", SDL_Color{174, 39, 39, 255}},
    {"Infinity", SDL_Color{204, 153, 0, 255}},

    // this / super
    {"this", SDL_Color{217, 130, 80, 255}},
    {"super", SDL_Color{217, 130, 80, 255}},

    // common built-ins / globals
    {"console", SDL_Color{65, 150, 217, 255}},
    {"document", SDL_Color{65, 150, 217, 255}},
    {"window", SDL_Color{65, 150, 217, 255}},
    {"parseInt", SDL_Color{65, 150, 217, 255}},
    {"parseFloat", SDL_Color{65, 150, 217, 255}},
    {"setTimeout", SDL_Color{65, 150, 217, 255}},
    {"setInterval", SDL_Color{65, 150, 217, 255}},
    {"fetch", SDL_Color{65, 150, 217, 255}},
};

inline std::unordered_map<std::string, SDL_Color> coloring_keywords = {};

class codeSection : public Section
{
private:
    std::string file_language = "none";

    std::unordered_map<std::string, SDL_Color> GetColoringKeywordsForCurrentFileLanguage()
    {
        std::string filename = GetOpenedFile();
        std::string language = filename.substr(filename.find_last_of(".") + 1);

        if (language == "cpp" || language == "c" || language == "h" || language == "hpp" || language == "h++" || language == "c++" || language == "cs" || language == "c#")
        {
            file_language = "cpp-style";
            return Cpp_style_coloring_keywords;
        }

        if (language == "py")
        {
            file_language = "python";
            return Python_style_coloring_keywords;
        }

        if (language == "js" || language == "ts")
        {
            file_language = "js_ts";
            return JavaScript_and_TypeScript_style_coloring_keywords;
        }

        return coloring_keywords;
    }

    std::vector<TextSpan> GetTextColoringSpans()
    {
        coloring_keywords = GetColoringKeywordsForCurrentFileLanguage();

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
            if (file_language == "cpp-style" || file_language == "js_ts")
            {
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
            }
            else if (file_language == "python")
            {
                if (text[i] == '#')
                {
                    size_t start = i;

                    while (i < text.size() && text[i] != '\n')
                    {
                        i++;
                    }

                    textSpans.push_back({start, i + 1, SDL_Color{62, 134, 78, 255}});
                    continue;
                }
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

            for (auto keyword : coloring_keywords)
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
