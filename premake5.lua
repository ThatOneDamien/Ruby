workspace "Ruby2D"
    architecture "x64"
    startproject "Sandbox"
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
group ""

group "Core"
	include "Ruby"
	include "Sandbox"
group ""