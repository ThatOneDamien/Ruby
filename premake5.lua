include("premake_tools")

workspace "Ruby"
    architecture "x64"
    startproject "RubyEditor"
    configurations
    {
        "Debug",
        "Release"
    }

    -- Final and intermediate directories for the build.
    outdir = "%{wks.location}/bin/%{cfg.system}/%{cfg.buildcfg}/%{prj.name}"
    intdir = "!%{wks.location}/bin/int/%{cfg.system}/%{cfg.buildcfg}/%{prj.name}"

    group "Dependencies"
        include "Dependencies/GLFW"
        include "Dependencies/glad"
        include "Dependencies/ImGui"
        include "Dependencies/FreeType"
        include "Dependencies/msdf-atlas-gen"
        include "Dependencies/soloud"
    group ""

    group "Core"
        include "Ruby"
        create_project("RubyEditor", "RubyEditor")
        create_project("Sandbox", "Sandbox")
    group ""
