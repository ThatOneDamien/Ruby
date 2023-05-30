project "glad"
	kind "StaticLib"
	language "C"

	targetdir ("bin/" .. outdir .. "/%{prj.name}")
	objdir ("bin/int/" .. outdir .. "/%{prj.name}")

	files
	{
		"include/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c"
	}

	includedirs "include"

	filter "system:linux"
		pic "on"
		systemversion "latest"

	filter "system:macosx"
		pic "on"

	filter "system:windows"
		systemversion "latest"
		staticruntime "on"

	filter { "system:windows", "configurations:Release" }
		buildoptions "/MT"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"