#pragma once


#ifdef _WIN32

#ifdef _WIN64
	// Windows x64 builds.
	#define RB_PLAT_WIND
	// For now OpenGL is the only Renderering API used.
	#define RB_USE_OPENGL

#else
	// Windows x86 builds.
	#error "x86/32-bit builds are not supported."
#endif

#else
	// Other builds. TODO handle more systems specifically.
	#error "Windows is the only platform currently supported by Ruby2D."

#endif

#include <stdint.h>
#include "Ruby/Main/Logging.h"

#include <filesystem>

#define RB_TRACE(msg, ...)      Ruby::Logger::getEngineLogger()->trace(msg, __VA_ARGS__);
#define RB_INFO(msg, ...)       Ruby::Logger::getEngineLogger()->info(msg, __VA_ARGS__);
#define RB_WARN(msg, ...)       Ruby::Logger::getEngineLogger()->warn(msg, __VA_ARGS__);
#define RB_ERROR(msg, ...)      Ruby::Logger::getEngineLogger()->error(msg, __VA_ARGS__);
#define RB_CRITICAL(msg, ...)   Ruby::Logger::getEngineLogger()->critical(msg, __VA_ARGS__);

#ifdef RB_PLAT_WIND
	#ifdef RB_DEBUG
		#define RB_ASSERTIONS
		#define RB_ASSERT(x, msg) { if(!(x)) { Ruby::Logger::getEngineLogger()->critical("Engine: %s", msg); __debugbreak(); } }
	#else
		#define RB_ASSERT(x, msg)
	#endif
#endif
