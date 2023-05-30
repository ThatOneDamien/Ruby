project "Sandbox"
	kind "ConsoleApp"
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
		"%{wks.location}/Ruby/Dependencies/glm"
	}

	links "Ruby"

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "RUBY_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RUBY_RELEASE"
		runtime "Release"
		optimize "on"