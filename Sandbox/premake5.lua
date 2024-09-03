project "Sandbox"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir (outdir)
    objdir (intdir)

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "%{wks.location}/Dependencies",
        "%{wks.location}/Ruby/src",
        "%{wks.location}/Dependencies/glm",
        "%{wks.location}/Dependencies/entt/include"
    }

    links
    {
        "Ruby",
        "glad",
        "GLFW",
        "ImGui",
        "msdf-atlas-gen",
        "msdfgen",
        "FreeType"
    }

    filter "system:windows"
        kind "WindowedApp"
        systemversion "latest"
        entrypoint "WinMainCRTStartup"
        links
        {
            "Soloud",
            "Dwmapi",
            "winmm"
        }
        if _ACTION == "gmake2" then
            defines "RB_GMAKE"
        end

    filter "system:linux"
        kind "ConsoleApp"
        disablewarnings "format-security"
        links { "Soloud", "pthread", "dl", "asound", "GL" }


    filter "configurations:Debug"
        defines "RB_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "RB_RELEASE"
        runtime "Release"
        optimize "on"
