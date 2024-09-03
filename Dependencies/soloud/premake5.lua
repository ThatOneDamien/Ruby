project "Soloud"
    kind "StaticLib"
    targetdir (outdir)
    objdir (intdir)
    language "C++"
    staticruntime "on"
    warnings "Off"

    files
    {
        "src/audiosource/**.c*",
        "src/filter/**.c*",
        "src/core/**.c*",
        "src/backend/null/**.c*"
    }

    includedirs
    {
        "src/**",
        "include"
    }

    defines "WITH_NULL"

    if (os.istarget("Windows")) then
        defines "WITH_WINMM"
        files "src/backend/winmm/**.c*"
    elseif (os.istarget("macosx")) then
        defines "WITH_COREAUDIO"
        files "src/backend/coreaudio/**.c*"
    else
        defines { "WITH_OSS", "WITH_ALSA" }
        files { "src/backend/oss/**.c*", "src/backend/alsa/**.c*" }
    end

    filter "configurations:Debug"
        runtime "Debug"
        defines "DEBUG"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        defines "NDEBUG"
        optimize "on"
