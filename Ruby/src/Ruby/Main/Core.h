#pragma once

#include "Platform.h"

#include <cstdint>
#include <cstdlib>
typedef uint32_t RendererID;

#include "Ruby/Main/Logging.h"

// Print a trace(lowest priority) message with the client logger
#define RB_CLIENT_TRACE(msg, ...)                          ::Ruby::Logger::getClientLogger()->trace(msg, ##__VA_ARGS__);
// Print an info message with the client logger
#define RB_CLIENT_INFO(msg, ...)                           ::Ruby::Logger::getClientLogger()->info(msg, ##__VA_ARGS__);
// Print a warning message with the client logger
#define RB_CLIENT_WARN(msg, ...)                           ::Ruby::Logger::getClientLogger()->warn(msg, ##__VA_ARGS__);
// Print an error message with the client logger
#define RB_CLIENT_ERROR(msg, ...)                          ::Ruby::Logger::getClientLogger()->error(msg, ##__VA_ARGS__);
// Print a critical(highest priority) message with the client logger
#define RB_CLIENT_CRITICAL(msg, ...)                       ::Ruby::Logger::getClientLogger()->critical(msg, ##__VA_ARGS__);
// Print a trace(lowest priority) message with the engine logger
#define RB_ENGINE_TRACE(msg, ...)                          ::Ruby::Logger::getEngineLogger()->trace(msg, ##__VA_ARGS__)
// Print an info message with the engine logger
#define RB_ENGINE_INFO(msg, ...)                           ::Ruby::Logger::getEngineLogger()->info(msg, ##__VA_ARGS__)
// Print a warning message with the engine logger
#define RB_ENGINE_WARN(msg, ...)                           ::Ruby::Logger::getEngineLogger()->warn(msg, ##__VA_ARGS__)
// Print an error message with the engine logger
#define RB_ENGINE_ERROR(msg, ...)                          ::Ruby::Logger::getEngineLogger()->error(msg, ##__VA_ARGS__)
// Print a critical(highest priority) message with the engine logger
#define RB_ENGINE_CRITICAL(msg, ...)                       ::Ruby::Logger::getEngineLogger()->critical(msg, ##__VA_ARGS__)


#ifdef RB_EXTERNAL_DEF
// Print a basic message with the color set by RB_LOG_SET_COLOR()
#define RB_LOG_BASIC(msg, ...)                      ::Ruby::Logger::getClientLogger()->basicLog(msg, ##__VA_ARGS__);
// Set the color of the logger for use in RB_LOG_BASIC() only
#define RB_LOG_SET_COLOR(textColor, highlightColor) ::Ruby::Logger::getClientLogger()->setLogColor(textColor, highlightColor);

// Print a trace(lowest priority) message with the client logger
#define RB_TRACE(msg, ...)                          ::Ruby::Logger::getClientLogger()->trace(msg, ##__VA_ARGS__);
// Print an info message with the client logger
#define RB_INFO(msg, ...)                           ::Ruby::Logger::getClientLogger()->info(msg, ##__VA_ARGS__);
// Print a warning message with the client logger
#define RB_WARN(msg, ...)                           ::Ruby::Logger::getClientLogger()->warn(msg, ##__VA_ARGS__);
// Print an error message with the client logger
#define RB_ERROR(msg, ...)                          ::Ruby::Logger::getClientLogger()->error(msg, ##__VA_ARGS__);
// Print a critical(highest priority) message with the client logger
#define RB_CRITICAL(msg, ...)                       ::Ruby::Logger::getClientLogger()->critical(msg, ##__VA_ARGS__);
#else                                                
// Print a basic message with the color set by RB_LOG_SET_COLOR()
#define RB_LOG_BASIC(msg, ...)                      ::Ruby::Logger::getEngineLogger()->basicLog(msg, ##__VA_ARGS__)
// Set the color of the logger for use in RB_LOG_BASIC() only
#define RB_LOG_SET_COLOR(textColor, highlightColor) ::Ruby::Logger::getEngineLogger()->setLogColor(textColor, highlightColor)

// Print a trace(lowest priority) message with the engine logger
#define RB_TRACE(msg, ...)                          ::Ruby::Logger::getEngineLogger()->trace(msg, ##__VA_ARGS__)
// Print an info message with the engine logger
#define RB_INFO(msg, ...)                           ::Ruby::Logger::getEngineLogger()->info(msg, ##__VA_ARGS__)
// Print a warning message with the engine logger
#define RB_WARN(msg, ...)                           ::Ruby::Logger::getEngineLogger()->warn(msg, ##__VA_ARGS__)
// Print an error message with the engine logger
#define RB_ERROR(msg, ...)                          ::Ruby::Logger::getEngineLogger()->error(msg, ##__VA_ARGS__)
// Print a critical(highest priority) message with the engine logger
#define RB_CRITICAL(msg, ...)                       ::Ruby::Logger::getEngineLogger()->critical(msg, ##__VA_ARGS__)
#endif

#ifdef RB_DEBUG

    // Prints error message and debug breaks unconditionally.
    // Same as doing RB_ASSERT(false, msg, ...) without a branch.
    // Works in both debug and release configurations. In release it doesn't
    // break the debugger.
    #define RB_ERROR_DEBUG(msg, ...) { RB_ERROR(msg, ##__VA_ARGS__); RB_DEBUG_BREAK(); }

    // Asserts that condition x is true, if x is false it prints error msg, debug breaks.
    // Only works in the debug configuration. In release it does nothing.
    #define RB_ASSERT(x, msg, ...) { if(!(x)) RB_ERROR_DEBUG(msg, ##__VA_ARGS__); }
    
#else
    #define RB_ERROR_DEBUG(msg, ...) { RB_ERROR(msg, ##__VA_ARGS__); }
    #define RB_ASSERT(x, msg, ...) { x; }
#endif

// Ensures that condition x is true, if x is false it prints error msg, and debug breaks(in debug mode). 
#define RB_ENSURE(x, msg, ...) { if(!(x)) RB_ERROR_DEBUG(msg, ##__VA_ARGS__); }

// Ensures that condition x is true, if x is false it prints critical msg, trips a breakpoint
// for the debugger(in debug mode), and terminates the program immediately.
#define RB_ENSURE_CRITICAL(x, msg, ...) { if(!(x)) { RB_ERROR_DEBUG(msg, ##__VA_ARGS__); exit(EXIT_FAILURE); } }

// Ensures that condition x is true, if x is false it prints error msg, debug breaks(in debug mode), and returns ret_val.
#define RB_ENSURE_RET(x, ret_val, msg, ...) { if(!(x)) { RB_ERROR_DEBUG(msg, ##__VA_ARGS__); return ret_val; } }

// Ensures that condition x is true, if x is false it prints error msg, debug breaks(in debug mode), and breaks from loop.
#define RB_ENSURE_BRK(x, msg, ...) { if(!(x)) { RB_ERROR_DEBUG(msg, ##__VA_ARGS__); break; } }

// Alias for RB_ENSURE_RET() where ret_val is absent, i.e. return type of function is void.
#define RB_ENSURE_RET_VOID(x, msg, ...) RB_ENSURE_RET(x, , msg, ##__VA_ARGS__)



#include <filesystem>
