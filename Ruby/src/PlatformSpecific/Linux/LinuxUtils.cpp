#include "ruby_pch.h"

#include "Ruby/Utility/PlatformFileUtils.h"
#include "Ruby/Main/App.h"
#include <cstring>

namespace Ruby
{
    namespace FileUtils
    {
        std::string loadFile(const char* filters)
        {
            char filename[1024];
            memset(filename, 0, 1024);
            FILE *f = popen("zenity --file-selection", "r");
            fgets(filename, 1024, f); 
            for(char* c = filename; *c != '\0'; ++c)
                if(*c == '\n')
                {
                    *c = '\0';
                    break;
                }
            fclose(f);
            return std::string(filename);
        }

        std::string saveFile()
        {
            char filename[1024];
            memset(filename, 0, 1024);
            FILE *f = popen("zenity --file-selection --save", "r");
            fgets(filename, 1024, f); 
            for(char* c = filename; *c != '\0'; ++c)
                if(*c == '\n')
                {
                    *c = '\0';
                    break;
                }
            fclose(f);
            return std::string(filename);
        }
    }
}
