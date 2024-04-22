project "Ruby"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin/int/" .. outdir .. "/%{prj.name}")
	
	pchheader "ruby_pch.h"
	pchsource "src/ruby_pch.cpp"

	files
	{
		"src/API/**.h",
		"src/API/**.cpp",
        "src/Ruby/**.h",
		"src/Ruby/**.cpp",
		"Dependencies/glm/**.hpp",
		"Dependencies/glm/**.inl",
		"Dependencies/stb_image/**.h",
		"Dependencies/stb_image/**.cpp"
	}

	includedirs
	{
		"src",
		"%{wks.location}/third_party/GLFW/include",
		"%{wks.location}/third_party/glad/include",
		"%{wks.location}/third_party/FreeType/include",
		"%{wks.location}/third_party/imgui",
		"%{wks.location}/third_party/soloud/include",
		"%{wks.location}/third_party/msdf-atlas-gen",
		"%{wks.location}/third_party/msdf-atlas-gen/msdfgen",
		"Dependencies/glm",
		"Dependencies/stb_image",
		"Dependencies/entt/include",
	}

	links
	{
		"GLFW",
		"glad",
		"ImGui",
		"msdf-atlas-gen",
		"SoloudStatic",
		"opengl32.lib"
	}

	defines
	{
		"GLFW_INCLUDE_NONE",
		"_CRT_SECURE_NO_WARNINGS"
	}

	filter "system:windows"
		systemversion "latest"
        files
        {
            "src/PlatformSpecific/Windows/**.h",
            "src/PlatformSpecific/Windows/**.cpp"
        }

	filter "configurations:Debug"
		defines "RB_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RB_RELEASE"
		runtime "Release"
		optimize "on"
