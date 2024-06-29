project "Sandbox"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin/int/" .. outdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
            "%{wks.location}/third_party",
        "%{wks.location}/Ruby/src",
        "%{wks.location}/Ruby/Dependencies/glm",
            "%{wks.location}/Ruby/Dependencies/entt/include"
    }
        links
        { 
            "Ruby", 
            "glad", 
            "GLFW", 
            "ImGui", 
            "msdf-atlas-gen", 
            "msdfgen", 
            "FreeType", 
            "SoloudStatic",
        }

    filter "system:windows"
        kind "WindowedApp"
        systemversion "latest"
            entrypoint "WinMainCRTStartup"
            links
            {   
                "Dwmapi",
                "winmm"
            }
            if _ACTION == "gmake2" then
                defines "RB_GMAKE"
            end

    filter "system:linux"
        kind "ConsoleApp"
        disablewarnings "format-security"
        

    filter "configurations:Debug"
        defines "RB_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "RB_RELEASE"
        runtime "Release"
        optimize "on"
