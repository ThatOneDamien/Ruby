set thispath=%~dp0%

call %thispath%..\Dependencies\premake\Windows\premake5.exe vs2022 --file=%thispath%..\premake5.lua

PAUSE
