#ifndef EXPORT_H
#define EXPORT_H

#if _WIN32
#define API_EXPORT __declspec (dllexport)
#define API_IMPORT __declspec (dllimport)
#elif __unix__
#define API_EXPORT __attribute__ ((visibility ("default")))
#define API_IMPORT
#else
#define API_EXPORT
#define API_IMPORT
#endif

#if !defined(FW_STATIC)
#if defined(FW_BUILD)
#define FW_API API_EXPORT
#else
#define FW_API API_IMPORT
#endif
#else
#define FW_API
#endif

#endif /* EXPORT_H */
