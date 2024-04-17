#pragma once


#ifdef _WIN32

	#define RB_NOVTABLE __declspec(novtable)

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
	#define RB_NOVTABLE  
#endif

#include <stdint.h>
typedef uint32_t RendererID;

#include "Ruby/Main/Logging.h"

#ifdef RB_EXTERNAL_DEF
#define RB_TRACE(msg, ...)                          ::Ruby::Logger::getClientLogger()->trace(msg, __VA_ARGS__);
#define RB_INFO(msg, ...)                           ::Ruby::Logger::getClientLogger()->info(msg, __VA_ARGS__);
#define RB_WARN(msg, ...)                           ::Ruby::Logger::getClientLogger()->warn(msg, __VA_ARGS__);
#define RB_ERROR(msg, ...)                          ::Ruby::Logger::getClientLogger()->error(msg, __VA_ARGS__);
#define RB_CRITICAL(msg, ...)                       ::Ruby::Logger::getClientLogger()->critical(msg, __VA_ARGS__);
#define RB_LOG_BASIC(msg, ...)                      ::Ruby::Logger::getClientLogger()->basicLog(msg, __VA_ARGS__);
#define RB_LOG_SET_COLOR(textColor, highlightColor) ::Ruby::Logger::getClientLogger()->setLogColor(textColor, highlightColor);
#else							                    
#define RB_TRACE(msg, ...)                          ::Ruby::Logger::getEngineLogger()->trace(msg, __VA_ARGS__);
#define RB_INFO(msg, ...)                           ::Ruby::Logger::getEngineLogger()->info(msg, __VA_ARGS__);
#define RB_WARN(msg, ...)                           ::Ruby::Logger::getEngineLogger()->warn(msg, __VA_ARGS__);
#define RB_ERROR(msg, ...)                          ::Ruby::Logger::getEngineLogger()->error(msg, __VA_ARGS__);
#define RB_CRITICAL(msg, ...)                       ::Ruby::Logger::getEngineLogger()->critical(msg, __VA_ARGS__);
#define RB_LOG_BASIC(msg, ...)                      ::Ruby::Logger::getEngineLogger()->basicLog(msg, __VA_ARGS__);
#define RB_LOG_SET_COLOR(textColor, highlightColor) ::Ruby::Logger::getEngineLogger()->setLogColor(textColor, highlightColor);
#endif

#ifdef RB_PLAT_WIND
	#ifdef RB_DEBUG

		// Assert that condition x is true, if it is false print error msg and debug break.
		#define RB_ASSERT(x, msg, ...) { if(!(x)) { ::Ruby::Logger::getEngineLogger()->error(msg, __VA_ARGS__); __debugbreak(); } }

		// Assert that condition x is true, if it is false print error msg, debug break, and return ret_val.
		#define RB_ASSERT_RET(x, ret_val, msg, ...) { if(!(x)) { ::Ruby::Logger::getEngineLogger()->error(msg, __VA_ARGS__); __debugbreak(); return ret_val; } }

		// Assert that condition x is true, if it is false print error msg, debug break, and break from loop.
		#define RB_ASSERT_BRK(x, msg, ...) { if(!(x)) { ::Ruby::Logger::getEngineLogger()->error(msg, __VA_ARGS__); __debugbreak(); break; } }
	#else
		#define RB_ASSERT(x, msg) x
		#define RB_ASSERT_RET(x, ret_val, msg, ...) { if(!(x)) { return ret_val; } }
		#define RB_ASSERT_BRK(x, msg, ...) { if(!(x)) { break; } }
	#endif
	// Alias for RB_ASSERT_RET() where ret_val is absent, i.e. return type of function is void.
	#define RB_ASSERT_RET_VOID(x, msg, ...) RB_ASSERT_RET(x, , msg, __VA_ARGS__)
#endif

#include <filesystem>