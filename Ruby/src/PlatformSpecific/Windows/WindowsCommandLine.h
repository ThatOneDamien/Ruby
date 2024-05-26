#pragma once

namespace Ruby 
{
    namespace CL 
    {
        PCHAR* CommandLineToArgvA(PCHAR CmdLine, int* _argc);
#ifdef RB_DEBUG
        bool RedirectConsoleIO();
        bool ReleaseConsole();
        void AdjustConsoleBuffer(int16_t minLength);
        bool CreateNewConsole(int16_t minLength);
#endif
    }
}