project "glad"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    warnings "Off"

    targetdir (outdir)
    objdir (intdir)

    files
    {
        "include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/glad.c"
    }

    includedirs "include"

    filter "system:linux"
        pic "on"
        systemversion "latest"

    filter "system:macosx"
        pic "on"

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
