cd %~dp0%..\

del Makefile
del *.vcxproj
del *.vcxproj.filters
del *.vcxproj.users
del *.sln

del Makefile /a /s
del *.vcxproj /a /s
del *.vcxproj.filters /a /s
del *.vcxproj.users /a /s
del *.sln /a /s

rmdir bin /s

PAUSE