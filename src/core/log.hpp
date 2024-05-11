//
// Created by tang on 23-2-5.
//

#ifndef GENERICFRAMEWORK_LOG_HPP
#define GENERICFRAMEWORK_LOG_HPP

#define LOG_LEVEL_TRACE 0
#define LOG_LEVEL_DEBUG 1
#define LOG_LEVEL_INFO  2
#define LOG_LEVEL_WARN  3
#define LOG_LEVEL_ERROR 4
#define LOG_LEVEL_FATAL 5
#define LOG_LEVEL_COUNT 6

#if !defined(LOG_LEVEL_DEFAULT)
#  if defined(NDEBUG)
#    define LOG_LEVEL_DEFAULT LOG_LEVEL_INFO
#  else
#    define LOG_LEVEL_DEFAULT LOG_LEVEL_TRACE
#  endif
#endif

#include "config.h"
#include "core/debug.h"
#include <string.h>
#include <stdio.h>

#define FLUSH_AND_HANG fflush(stdout); fflush(stderr); debug_break()

#if _WIN32
#  define FILE_NAME_ONLY (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#  define FILE_NAME_ONLY (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#if defined(__cplusplus)
extern "C"
#endif

/**
 * this api is internal use, do NOT call directly!!
 * @param file source file
 * @param line source line
 * @param level log level
 * @param msg message pattern
 * @param ... arguments
 */

void FW_API log_printf(const char *file, s32 line, s32 level, const char *msg, ...);

#if LOG_LEVEL_DEFAULT <= LOG_LEVEL_TRACE
#  define L_TRACE(MSG,...) do { log_printf(FILE_NAME_ONLY, __LINE__, LOG_LEVEL_TRACE, MSG, ##__VA_ARGS__); } while (0)
#else
#  define L_TRACE(MSG,...) (void)(MSG, ##__VA_ARGS__)
#endif

#if LOG_LEVEL_DEFAULT <= LOG_LEVEL_DEBUG
#  define L_DEBUG(MSG,...) do { log_printf(FILE_NAME_ONLY, __LINE__, LOG_LEVEL_DEBUG, MSG, ##__VA_ARGS__); } while (0)
#else
#  define L_DEBUG(MSG,...) (void)(MSG, ##__VA_ARGS__)
#endif

#if LOG_LEVEL_DEFAULT <= LOG_LEVEL_INFO
#  define L_INFO(MSG,...) do { log_printf(FILE_NAME_ONLY, __LINE__, LOG_LEVEL_INFO, MSG, ##__VA_ARGS__); } while (0)
#else
#  define L_INFO(MSG,...) (void)(MSG, ##__VA_ARGS__)
#endif

#if LOG_LEVEL_DEFAULT <= LOG_LEVEL_WARN
#  define L_WARN(MSG,...) do { log_printf(FILE_NAME_ONLY, __LINE__, LOG_LEVEL_WARN, MSG, ##__VA_ARGS__); } while (0)
#else
#  define L_WARN(MSG,...) (void)(MSG, ##__VA_ARGS__)
#endif

#define L_ERROR(MSG,...) do { log_printf(FILE_NAME_ONLY, __LINE__, LOG_LEVEL_ERROR, MSG, ##__VA_ARGS__); } while (0)
#define L_FATAL(MSG,...) do { log_printf(FILE_NAME_ONLY, __LINE__, LOG_LEVEL_FATAL, MSG, ##__VA_ARGS__); FLUSH_AND_HANG; } while (0)

/**
 * conditional log macros
 */
#define LC_TRACE(COND, MSG, ...) do { if (COND) L_TRACE(MSG, ##__VA_ARGS__); } while(0)
#define LC_DEBUG(COND, MSG, ...) do { if (COND) L_DEBUG(MSG, ##__VA_ARGS__); } while(0)
#define LC_INFO(COND, MSG, ...)  do { if (COND) L_INFO(MSG, ##__VA_ARGS__);  } while(0)
#define LC_WARN(COND, MSG, ...)  do { if (COND) L_WARN(MSG, ##__VA_ARGS__);  } while(0)
#define LC_ERROR(COND, MSG, ...) do { if (COND) L_ERROR(MSG, ##__VA_ARGS__); } while(0)
#define LC_FATAL(COND, MSG, ...) do { if (COND) L_FATAL(MSG, ##__VA_ARGS__); } while(0)

#endif // GENERICFRAMEWORK_LOG_HPP
