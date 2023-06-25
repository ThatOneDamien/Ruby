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
		"src/**.h",
		"src/**.cpp",
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
		"%{wks.location}/third_party/ImGui",
		"Dependencies/glm",
		"Dependencies/stb_image"
	}

	links
	{
		"GLFW",
		"glad",
		"ImGui",
		"FreeType",
		"opengl32.lib"
	}

	defines
	{
		"GLFW_INCLUDE_NONE",
		"_CRT_SECURE_NO_WARNINGS"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "RB_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RB_RELEASE"
		runtime "Release"
		optimize "on"
