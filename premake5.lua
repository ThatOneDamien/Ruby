workspace "Ruby"
    architecture "x64"
    startproject "RubyEditor"
    configurations
    {
        "Debug",
        "Release"
    }

    -- Final and intermediate directories for the build.
    outdir = "%{wks.location}/bin/%{cfg.system}/%{cfg.buildcfg}"
    intdir = "%{wks.location}/bin/int/%{cfg.system}/%{cfg.buildcfg}"


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
        include "RubyEditor"
        include "Sandbox"
    group ""
