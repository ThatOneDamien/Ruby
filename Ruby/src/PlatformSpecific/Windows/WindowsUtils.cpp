#include "ruby_pch.h"

#include "Ruby/Utility/PlatformFileUtils.h"
#include "Ruby/Main/App.h"

#include <commdlg.h>

namespace Ruby
{
    namespace FileUtils
    {
        std::string loadFile(const char* filters)
        {
            constexpr size_t MAX_PATH_SIZE = 256ull;

            OPENFILENAMEA ofn;
            CHAR pathBuf[MAX_PATH_SIZE] = { '\0' };
            CHAR currentDir[MAX_PATH_SIZE] = { '\0' };

            ZeroMemory(&ofn, sizeof(OPENFILENAME));

            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = (HWND)App::getInstance().getWindow().getNativeWindowPtr();
            ofn.lpstrFile = pathBuf;
            ofn.nMaxFile = MAX_PATH_SIZE;
            ofn.lpstrInitialDir = GetCurrentDirectoryA(MAX_PATH_SIZE, currentDir) ? currentDir : nullptr;
            ofn.lpstrFilter = filters;
            ofn.nFilterIndex = 1;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

            if (GetOpenFileNameA(&ofn))
                return ofn.lpstrFile;

            return std::string();
        }
    }
}
