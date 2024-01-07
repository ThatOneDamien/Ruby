project "FreeType"
	kind "StaticLib"
	language "C"
	cppdialect "C++14"

	targetdir ("bin/" .. outdir .. "/%{prj.name}")
	objdir ("bin/int/" .. outdir .. "/%{prj.name}")

	files
	{
		"src/autofit/autofit.c",
        "src/bdf/bdf.c",
        "src/cff/cff.c",
        "src/base/ftbase.c",
        "src/base/ftbitmap.c",
        "src/cache/ftcache.c",
        "src/base/ftfstype.c",
        "src/base/ftglyph.c",
        "src/gzip/ftgzip.c",
        "src/base/ftinit.c",
        "src/lzw/ftlzw.c",
        "src/base/ftstroke.c",
        "src/sdf/sdf.c",
        "src/svg/svg.c",
        "src/smooth/smooth.c",

        "src/base/ftbbox.c",
        "src/base/ftgxval.c",
        "src/base/ftlcdfil.c",
        "src/base/ftmm.c",
        "src/base/ftotval.c",
        "src/base/ftpatent.c",
        "src/base/ftpfr.c",
        "src/base/ftsynth.c",
        "src/base/ftfstype.c",
        "src/pcf/pcf.c",
        "src/pfr/pfr.c",
        "src/psaux/psaux.c",
        "src/pshinter/pshinter.c",
        "src/psnames/psmodule.c",
        "src/raster/raster.c",
        "src/sfnt/sfnt.c",
        "src/truetype/truetype.c",
        "src/type1/type1.c",
        "src/cid/type1cid.c",
        "src/type42/type42.c",
        "src/winfonts/winfnt.c"
	}

	includedirs "include"

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"FT2_BUILD_LIBRARY",
		"VC_EXTRALEAN"
	}

	filter "configurations:Release"
		defines "NDEBUG"

	filter "configurations:Debug"
		defines "_DEBUG"

	filter "system:linux"
		pic "on"
		systemversion "latest"
		files "builds/unix/ftsystem.c"

	filter "system:macosx"
		pic "on"
		files "src/base/ftsystem.c"

	filter "system:windows"
		systemversion "latest"
		staticruntime "on"
		defines 
		{ 
			"WIN32", 
			"WIN32_LEAN_AND_MEAN"
		}
		files 
		{
			"builds/windows/ftsystem.c", 
			"src/base/ftwinfnt.c" 
		}

	filter { "system:windows", "configurations:Release" }
		buildoptions "/MT"

	filter { "system:windows", "configurations:Debug" }
		buildoptions "/MTd"
		files "builds/windows/ftdebug.c"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"