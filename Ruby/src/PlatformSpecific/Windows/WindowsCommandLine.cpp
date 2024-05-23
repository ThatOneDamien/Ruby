#include "ruby_pch.h"

#include <Windows.h>
#include "WindowsCommandLine.h"

namespace Ruby {
    namespace CL {
        PCHAR*
            CommandLineToArgvA(
                PCHAR CmdLine,
                int* _argc
            )
        {
            PCHAR* argv;
            PCHAR  _argv;
            ULONG   len;
            ULONG   argc;
            CHAR   a;
            ULONG   i, j;

            BOOLEAN  in_QM;
            BOOLEAN  in_TEXT;
            BOOLEAN  in_SPACE;

            len = (ULONG)strlen(CmdLine);
            i = ((len + 2) / 2) * sizeof(PVOID) + sizeof(PVOID);

            argv = (PCHAR*)GlobalAlloc(GMEM_FIXED,
                i + (len + 2) * sizeof(CHAR));

            _argv = (PCHAR)(((PUCHAR)argv) + i);

            argc = 0;

            if (!argv)
                return nullptr;

            argv[argc] = _argv;
            in_QM = FALSE;
            in_TEXT = FALSE;
            in_SPACE = TRUE;
            i = 0;
            j = 0;

            while (a = CmdLine[i]) {
                if (in_QM) {
                    if (a == '\"') {
                        in_QM = FALSE;
                    }
                    else {
                        _argv[j] = a;
                        j++;
                    }
                }
                else {
                    switch (a) {
                    case '\"':
                        in_QM = TRUE;
                        in_TEXT = TRUE;
                        if (in_SPACE) {
                            argv[argc] = _argv + j;
                            argc++;
                        }
                        in_SPACE = FALSE;
                        break;
                    case ' ':
                    case '\t':
                    case '\n':
                    case '\r':
                        if (in_TEXT) {
                            _argv[j] = '\0';
                            j++;
                        }
                        in_TEXT = FALSE;
                        in_SPACE = TRUE;
                        break;
                    default:
                        in_TEXT = TRUE;
                        if (in_SPACE) {
                            argv[argc] = _argv + j;
                            argc++;
                        }
                        _argv[j] = a;
                        j++;
                        in_SPACE = FALSE;
                        break;
                    }
                }
                i++;
            }
            _argv[j] = '\0';
            argv[argc] = NULL;

            (*_argc) = argc;
            return argv;
        }

#ifdef RB_DEBUG
        bool RedirectConsoleIO()
        {
            bool result = true;
            FILE* fp;

            // Redirect STDIN if the console has an input handle
            if (GetStdHandle(STD_INPUT_HANDLE) != INVALID_HANDLE_VALUE)
                if (freopen_s(&fp, "CONIN$", "r", stdin) != 0)
                    result = false;
                else
                    setvbuf(stdin, NULL, _IONBF, 0);

            // Redirect STDOUT if the console has an output handle
            if (GetStdHandle(STD_OUTPUT_HANDLE) != INVALID_HANDLE_VALUE)
                if (freopen_s(&fp, "CONOUT$", "w", stdout) != 0)
                    result = false;
                else
                    setvbuf(stdout, NULL, _IONBF, 0);

            // Redirect STDERR if the console has an error handle
            if (GetStdHandle(STD_ERROR_HANDLE) != INVALID_HANDLE_VALUE)
                if (freopen_s(&fp, "CONOUT$", "w", stderr) != 0)
                    result = false;
                else
                    setvbuf(stderr, NULL, _IONBF, 0);

            // Make C++ standard streams point to console as well.
            std::ios::sync_with_stdio(true);

            // Clear the error state for each of the C++ standard streams.
            std::wcout.clear();
            std::cout.clear();
            std::wcerr.clear();
            std::cerr.clear();
            std::wcin.clear();
            std::cin.clear();

            return result;
        }

        bool ReleaseConsole()
        {
            bool result = true;
            FILE* fp;

            // Just to be safe, redirect standard IO to NUL before releasing.

            // Redirect STDIN to NUL
            if (freopen_s(&fp, "NUL:", "r", stdin) != 0)
                result = false;
            else
                setvbuf(stdin, NULL, _IONBF, 0);

            // Redirect STDOUT to NUL
            if (freopen_s(&fp, "NUL:", "w", stdout) != 0)
                result = false;
            else
                setvbuf(stdout, NULL, _IONBF, 0);

            // Redirect STDERR to NUL
            if (freopen_s(&fp, "NUL:", "w", stderr) != 0)
                result = false;
            else
                setvbuf(stderr, NULL, _IONBF, 0);

            // Detach from console
            if (!FreeConsole())
                result = false;

            return result;
        }

        void AdjustConsoleBuffer(int16_t minLength)
        {
            // Set the screen buffer to be big enough to scroll some text
            CONSOLE_SCREEN_BUFFER_INFO conInfo;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &conInfo);
            if (conInfo.dwSize.Y < minLength)
                conInfo.dwSize.Y = minLength;
            SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), conInfo.dwSize);
        }

        bool CreateNewConsole(int16_t minLength)
        {
            bool result = false;

            // Release any current console and redirect IO to NUL
            ReleaseConsole();

            // Attempt to create new console
            if (AllocConsole())
            {
                AdjustConsoleBuffer(minLength);
                result = RedirectConsoleIO();
            }

            return result;
        }
#endif
    }
}