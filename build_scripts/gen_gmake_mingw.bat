set thispath=%~dp0%

call %thispath%..\third_party\premake\Windows\premake5.exe gmake2 --file=%thispath%..\premake5.lua --cc=mingw

PAUSE
