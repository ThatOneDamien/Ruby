VulkanSDKLocation = os.getenv('VULKAN_SDK')

function create_project(ProjectName, ProjectDirectory)
    project (ProjectName)
        language "C++"
        cppdialect "C++17"
        staticruntime "on"
        location (ProjectDirectory)

        targetdir (outdir)
        objdir (intdir)

        libdirs (VulkanSDKLocation .. "/lib")

        files
        {
            ProjectDirectory .. "/src/**.h",
            ProjectDirectory .. "/src/**.cpp"
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
            "FreeType",
            "Soloud",
            "glslang",
            "OSDependent",
            "MachineIndependent",
            "GenericCodeGen",
            "glslang-default-resource-limits",
            "SPIRV",
            "SPIRV-Tools-opt",
            "SPIRV-Tools",
        }

        filter "system:windows"
            kind "WindowedApp"
            systemversion "latest"
            entrypoint "WinMainCRTStartup"
            links
            {
                "Dwmapi",
                "winmm",
            }
            if _ACTION == "gmake2" then
                defines "RB_GMAKE"
            end

        filter "system:linux"
            kind "ConsoleApp"
            disablewarnings "format-security"
            links { "asound", "pthread", "dl", "GL" }


        filter "configurations:Debug"
            defines "RB_DEBUG"
            runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            defines "RB_RELEASE"
            runtime "Release"
            optimize "on"

end
