project "Ruby"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir (outdir)
    objdir (intdir)
    
    pchheader "ruby_pch.h"
    pchsource "src/ruby_pch.cpp"

    files
    {
        "src/API/**.h",
        "src/API/**.cpp",
        "src/Ruby/**.h",
        "src/Ruby/**.cpp",
        "%{wks.location}/Dependencies/stb_image/**.cpp"
    }

    includedirs
    {
        "src",
        "%{wks.location}/Dependencies/GLFW/include",
        "%{wks.location}/Dependencies/glad/include",
        "%{wks.location}/Dependencies/FreeType/include",
        "%{wks.location}/Dependencies/ImGui",
        "%{wks.location}/Dependencies/soloud/include",
        "%{wks.location}/Dependencies/msdf-atlas-gen",
        "%{wks.location}/Dependencies/msdf-atlas-gen/msdfgen",
        "%{wks.location}/Dependencies/glm",
        "%{wks.location}/Dependencies/stb_image",
        "%{wks.location}/Dependencies/entt/include",
    }

    links
    {
        "GLFW",
        "glad",
        "ImGui",
        "msdf-atlas-gen",
        "Soloud"
    }

    defines
    {
        "GLFW_INCLUDE_NONE",
        "_CRT_SECURE_NO_WARNINGS"
    }

    filter "system:windows"
        systemversion "latest"
        links "opengl32.lib"
        files
        {
            "src/PlatformSpecific/Windows/**.h",
            "src/PlatformSpecific/Windows/**.cpp"
        }
        if _ACTION == "vs2022" then
            buildoptions "/Yc\"ruby_pch.h\""
        elseif _ACTION == "gmake2" then
            defines "RB_GMAKE"
        end

    filter "system:linux"
        files
        {
            "src/PlatformSpecific/Linux/**.h",
            "src/PlatformSpecific/Linux/**.cpp"
        }
        disablewarnings "format-security"

    filter "configurations:Debug"
        defines "RB_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "RB_RELEASE"
        runtime "Release"
        optimize "on"
