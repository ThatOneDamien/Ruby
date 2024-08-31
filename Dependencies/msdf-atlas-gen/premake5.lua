project "msdfgen"
    kind "StaticLib"
    location "msdfgen"
    language "C++"
    cppdialect "C++11"
    staticruntime "on"
    warnings "Off"

    targetdir (outdir)
    objdir (intdir)

    files
    {
        "msdfgen/core/**.h",
        "msdfgen/core/**.hpp",
        "msdfgen/core/**.cpp",
        "msdfgen/ext/**.h",
        "msdfgen/ext/**.hpp",
        "msdfgen/ext/**.cpp",
        "msdfgen/lib/**.cpp",
        "msdfgen/include/**.h"
    }

    includedirs
    {
        "msdfgen/include",
        "../FreeType/include"
    }

    defines
    {
        "MSDFGEN_USE_CPP11"
    }

    links
    {
        "FreeType"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

project "msdf-atlas-gen"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    warnings "Off"

    targetdir (outdir)
    objdir (intdir)

    files
    {
        "msdf-atlas-gen/**.h",
        "msdf-atlas-gen/**.hpp",
        "msdf-atlas-gen/**.cpp"
    }

    includedirs
    {
        "msdf-atlas-gen",
        "artery-font-format",
        "msdfgen",
        "msdfgen/include"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    links
    {
        "msdfgen"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
