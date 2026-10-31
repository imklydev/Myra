#include "rendering.h"

#include <cstdint>
#include <unordered_set>
#include <utility>

// Color
void SetDrawColor(Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

// Shapes
void DrawRect(Vector2 pos, Vector2 size, bool filled)
{
    SDL_FRect rect = {pos.x, pos.y, size.x, size.y};

    if (filled)
    {
        SDL_RenderFillRect(renderer, &rect);
    }
    else
    {
        SDL_RenderRect(renderer, &rect);
    }
}

// Text

std::unordered_map<std::string, TTF_Font *> cachedFonts;

namespace
{
    struct LineKey
    {
        std::string line;
        int textSize;
        Uint8 r, g, b, a;

        bool operator==(const LineKey &other) const
        {
            return textSize == other.textSize && r == other.r && g == other.g &&
                   b == other.b && a == other.a && line == other.line;
        }
    };

    struct LineKeyHash
    {
        size_t operator()(const LineKey &key) const noexcept
        {
            size_t hash = std::hash<std::string>{}(key.line);

            auto combine = [&hash](size_t value)
            {
                hash ^= value + 0x9e3779b97f4a7c15ULL + (hash << 6) + (hash >> 2);
            };

            combine(std::hash<int>{}(key.textSize));
            combine(key.r);
            combine(key.g);
            combine(key.b);
            combine(key.a);

            return hash;
        }
    };

    struct CachedLine
    {
        SDL_Texture *texture = nullptr;
        float width = 0;
        float height = 0;
        size_t bytes = 0;
        uint64_t lastUsed = 0;
    };

    constexpr size_t MAX_CACHED_LINE_BYTES = 32 * 1024 * 1024;

    std::unordered_map<LineKey, CachedLine, LineKeyHash> cachedLines;
    size_t cachedLineBytes = 0;
    uint64_t textUseClock = 0;

    TTF_Font *GetFont(int textSize)
    {
        std::string fontKey = FONT + "_" + std::to_string(textSize);

        auto it = cachedFonts.find(fontKey);
        if (it != cachedFonts.end())
        {
            return it->second;
        }

        TTF_Font *font = TTF_OpenFont(FONT.c_str(), textSize);
        if (!font)
        {
            return nullptr;
        }

        cachedFonts[fontKey] = font;
        return font;
    }

    void EvictLines()
    {
        while (cachedLineBytes > MAX_CACHED_LINE_BYTES && cachedLines.size() > 1)
        {
            auto victim = cachedLines.begin();

            for (auto it = cachedLines.begin(); it != cachedLines.end(); ++it)
            {
                if (it->second.lastUsed < victim->second.lastUsed)
                {
                    victim = it;
                }
            }

            cachedLineBytes -= victim->second.bytes;
            SDL_DestroyTexture(victim->second.texture);
            cachedLines.erase(victim);
        }
    }

    SDL_Texture *GetLineTexture(
        TTF_Font *font, const LineKey &key, float &width, float &height)
    {
        auto it = cachedLines.find(key);
        if (it != cachedLines.end())
        {
            it->second.lastUsed = ++textUseClock;
            width = it->second.width;
            height = it->second.height;
            return it->second.texture;
        }

        SDL_Color color = {key.r, key.g, key.b, key.a};
        SDL_Surface *surface =
            TTF_RenderText_Blended(font, key.line.c_str(), key.line.size(), color);

        if (!surface)
        {
            return nullptr;
        }

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_DestroySurface(surface);

        if (!texture)
        {
            return nullptr;
        }

        float textureWidth = 0;
        float textureHeight = 0;
        SDL_GetTextureSize(texture, &textureWidth, &textureHeight);

        CachedLine cachedLine;
        cachedLine.texture = texture;
        cachedLine.width = textureWidth;
        cachedLine.height = textureHeight;
        cachedLine.bytes =
            static_cast<size_t>(textureWidth) * static_cast<size_t>(textureHeight) * 4;
        cachedLine.lastUsed = ++textUseClock;

        cachedLineBytes += cachedLine.bytes;
        cachedLines.emplace(key, cachedLine);

        width = textureWidth;
        height = textureHeight;
        return texture;
    }

    void SplitLines(
        const std::string &text, std::unordered_set<std::string> &lines)
    {
        size_t start = 0;

        while (start <= text.size())
        {
            size_t end = text.find('\n', start);
            if (end == std::string::npos)
            {
                end = text.size();
            }

            std::string line = text.substr(start, end - start);
            if (!line.empty() && line.back() == '\r')
            {
                line.pop_back();
            }

            if (!line.empty())
            {
                lines.insert(std::move(line));
            }

            if (end == text.size())
            {
                break;
            }

            start = end + 1;
        }
    }
}

void DrawText(const std::string &text, int textSize, Vector2 pos, Vector2 size)
{
    SDL_Color color;
    SDL_GetRenderDrawColor(
        renderer, &color.r, &color.g, &color.b, &color.a);

    TTF_Font *font = GetFont(textSize);
    if (!font)
    {
        return;
    }

    int lineSkip = TTF_GetFontLineSkip(font);

    bool clip = size.x >= 0 && size.y >= 0;
    bool hadClip = clip && SDL_RenderClipEnabled(renderer);
    SDL_Rect previousClip = {0, 0, 0, 0};

    if (clip)
    {
        if (hadClip)
        {
            SDL_GetRenderClipRect(renderer, &previousClip);
        }

        SDL_Rect clipRect = {
            static_cast<int>(pos.x),
            static_cast<int>(pos.y),
            static_cast<int>(size.x),
            static_cast<int>(size.y)};

        SDL_SetRenderClipRect(renderer, &clipRect);
    }

    float y = pos.y;
    size_t start = 0;

    while (start <= text.size())
    {
        size_t end = text.find('\n', start);
        if (end == std::string::npos)
        {
            end = text.size();
        }

        std::string line = text.substr(start, end - start);
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        if (!line.empty())
        {
            LineKey key = {
                std::move(line), textSize, color.r, color.g, color.b, color.a};

            float width = 0;
            float height = 0;
            SDL_Texture *texture = GetLineTexture(font, key, width, height);

            if (texture)
            {
                SDL_FRect textRect = {pos.x, y, width, height};
                SDL_RenderTexture(renderer, texture, nullptr, &textRect);
            }
        }

        if (end == text.size())
        {
            break;
        }

        start = end + 1;
        y += lineSkip;

        if (clip && y >= pos.y + size.y)
        {
            break;
        }
    }

    if (clip)
    {
        if (hadClip)
        {
            SDL_SetRenderClipRect(renderer, &previousClip);
        }
        else
        {
            SDL_SetRenderClipRect(renderer, nullptr);
        }
    }

    EvictLines();
}

void FreeText(const std::string &text, int textSize)
{
    std::unordered_set<std::string> lines;
    SplitLines(text, lines);

    for (auto it = cachedLines.begin(); it != cachedLines.end();)
    {
        const LineKey &key = it->first;

        if (key.textSize == textSize && lines.find(key.line) != lines.end())
        {
            cachedLineBytes -= it->second.bytes;
            SDL_DestroyTexture(it->second.texture);
            it = cachedLines.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void FreeAllText()
{
    for (auto &entry : cachedLines)
    {
        SDL_DestroyTexture(entry.second.texture);
    }

    cachedLines.clear();
    cachedLineBytes = 0;
}

void FreeAllFonts()
{
    for (auto &entry : cachedFonts)
    {
        TTF_CloseFont(entry.second);
    }

    cachedFonts.clear();
}
