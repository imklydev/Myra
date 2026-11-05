#pragma once

#include <filesystem>
#include <sys/stat.h>

#include "../section.h"
#include "../screens/codeEditorScreen.h"

namespace fs = std::filesystem;

inline std::unordered_map<std::string, bool> isFolderCollapsed;
inline std::string currentFolder;
inline char *currentFile;
inline void collapseCurrentFolder()
{
    isFolderCollapsed[currentFolder] = !isFolderCollapsed[currentFolder];
}

inline void openFile()
{
    OpenTab(currentFile);
}

class projectFolderOverview : public Section
{
private:
    Vector2 textOffset = {0, 0};

    std::string getFileTitle(std::string path)
    {
        return path.substr(path.find_last_of("/\\") + 1);
    }

    void ShowFilesInFolder(std::string path)
    {
        for (const auto &entry : fs::directory_iterator(path))
        {

            // Converting the path to const char * in the
            // subsequent lines
            fs::path file = entry.path();
            std::string filename = file.string();
            const char *path = filename.c_str();

            // Testing whether the path points to a
            // non-directory or not If it does, displays path
            if (stat(path, &sb) == 0 && !(sb.st_mode & S_IFDIR))
            {
                currentFile = (char *)path;

                Button openFileBtn = {Vector2{15, 65.0f + 25.0f} + textOffset + pos, {size.x - 15 - textOffset.x, 25}, openFile};
                openFileBtn.update();

                if (isMouseHoveringOver(Vector2{15, 65.0f + 25.0f} + textOffset + pos, {size.x - 15 - textOffset.x, 25}))
                {
                    SetDrawColor(TABS_SECTION_SELECTED_TAB_COLOR);
                    DrawRect(Vector2{15, 65.0f + 25.0f} + textOffset + pos, {size.x - 15 - textOffset.x, 25});
                    SetDrawColor(TEXT_COLOR);
                }

                textOffset += Vector2(0, 25);
                DrawRect(Vector2{0, 65.0f} + textOffset + pos, {1, 25}); // draw the line before the file
                DrawText(getFileTitle(filename), 16, Vector2{15, 65.0f} + textOffset + pos, {size.x - 15 - textOffset.x, 100});
            }
            else
            {
                currentFolder = filename;
                textOffset += Vector2(0, 25);

                Button openFolderBtn = {Vector2{0, 65.0f} + textOffset + pos, {15, 25}, collapseCurrentFolder};
                openFolderBtn.update();

                DrawText(getFileTitle(filename), 16, Vector2{15, 65.0f} + textOffset + pos, {size.x - 15 - textOffset.x, 100});

                if (!isFolderCollapsed[currentFolder])
                {
                    DrawImage(Vector2{-10, 65.0f} + textOffset + pos, {-1, -1}, DROPDOWN_FOLDER_ICON.c_str());

                    textOffset += Vector2(15, 0);
                    ShowFilesInFolder(filename);
                    textOffset -= Vector2(15, 0);
                }
                else
                {
                    DrawImage(Vector2{-10, 65.0f} + textOffset + pos, {-1, -1}, COLLAPSE_FOLDER_ICON.c_str());
                }
            }
        }
    }

public:
    Vector2 pos = {0, 0};
    Vector2 size = {300, 5000};
    float textSize = 16;
    struct stat sb;

    void draw(Vector2 pos_offset) override
    {
        std::string openedFolder = GetOpenedFolder();
        SetDrawColor(TABS_SECTION_TAB_COLOR);
        DrawRect(pos + pos_offset, size);

        SetDrawColor(TEXT_COLOR);

        if (openedFolder.empty())
        {
            DrawText("Nothing to see here yet...\nTry opening a folder with\nCTRL+SHIFT+O", 16, Vector2{15, 50} + pos);
        }
        else
        {
            DrawText(getFileTitle(openedFolder), 16, Vector2{15, 50} + pos);

            textOffset = Vector2{15, 0};
            ShowFilesInFolder(openedFolder);
        }

        if (isMouseHoveringOver(pos + pos_offset, size))
        {
            pos.y += Mouse.scroll * SCROLL_SENSITIVITY;

            if (pos.y > 0)
            {
                pos.y = 0;
            }
        }
    }
};

inline projectFolderOverview ProjectFolderOverview;
