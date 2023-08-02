workspace "Ruby2D"
    architecture "x64"
    startproject "RubyEditor"
    configurations
    {
        "Debug",
        "Release"
    }

outdir = "%{cfg.system}%{cfg.architecture}/%{cfg.buildcfg}"

group "Dependencies"
	include "third_party/GLFW"
	include "third_party/glad"
	include "third_party/ImGui"
    include "third_party/FreeType"
group ""

group "Core"
	include "Ruby"
    include "RubyEditor"
    include "TestGame"
	include "Sandbox"
group ""