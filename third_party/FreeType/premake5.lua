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
		"src/cff/cff.c",
		"src/base/ftbase.c",
		"src/base/ftbitmap.c",
		"src/cache/ftcache.c",
		"src/base/ftfstype.c",
		"src/base/ftgasp.c",
		"src/base/ftglyph.c",
		"src/gzip/ftgzip.c",
		"src/base/ftinit.c",
		"src/lzw/ftlzw.c",
		"src/base/ftstroke.c",
		"src/base/ftsystem.c",
		"src/smooth/smooth.c",
		"src/base/ftbbox.c",
		"src/base/ftmm.c",
		"src/base/ftpfr.c",
		"src/base/ftsynth.c",
		"src/base/fttype1.c",
		"src/base/ftwinfnt.c",
		"src/base/ftlcdfil.c",
		"src/base/ftgxval.c",
		"src/base/ftotval.c",
		"src/base/ftpatent.c",
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
		"src/winfonts/winfnt.c",
		"include/ft2build.h",
		"include/freetype/config/ftconfig.h",
		"include/freetype/config/ftheader.h",
		"include/freetype/config/ftmodule.h",
		"include/freetype/config/ftoption.h",
		"include/freetype/config/ftstdlib.h"
	}

	includedirs "include"

	defines
	{
		"_LIB",
		"_CRT_SECURE_NO_WARNINGS",
		"FT2_BUILD_LIBRARY"
	}

	filter "configurations:Release"
		defines "NDEBUG"

	filter "configurations:Debug"
		defines "_DEBUG"

	filter "system:linux"
		pic "on"
		systemversion "latest"

	filter "system:macosx"
		pic "on"

	filter "system:windows"
		systemversion "latest"
		staticruntime "on"
		defines "WIN32"
		files "builds/windows/ftdebug.c"

	filter { "system:windows", "configurations:Release" }
		buildoptions "/MT"

	filter { "system:windows", "configurations:Debug" }
		buildoptions "/MTd"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"