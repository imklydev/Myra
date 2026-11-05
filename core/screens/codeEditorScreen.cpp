#include "codeEditorScreen.h"

void ToggleShowProjectFolderOverview()
{
    CodeEditorScreen.shouldShowProjectFolderOverview = !CodeEditorScreen.shouldShowProjectFolderOverview;
}
std::string GetOpenedFolder()
{
    return CodeEditorScreen.openedFolder;
}

void OpenTab(char *file)
{
    CodeEditorScreen.OpenTab(file);
}

void SetCurrentTab(int tab)
{
    CodeEditorScreen.SetCurrentTab(tab);
}