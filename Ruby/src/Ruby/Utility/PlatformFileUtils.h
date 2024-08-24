#pragma once

#include <string>

namespace Ruby
{
    namespace FileUtils
    {
        std::string loadFile(const char* filters);
        std::string saveFile(const char* filters);
    }
}
