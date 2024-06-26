//
// Created by tang on 22-6-8.
//

#ifndef INFOSTATION_PATH_TOOLS_HPP
#define INFOSTATION_PATH_TOOLS_HPP

#include "config.h"
#include <string_view>
#include <string>

/**
 * 获取EXE/ELF文件名，不包含后缀
 */
FW_API std::string_view GetAppExecutableName();

/**
 * 获取EXE/ELF文件完整路径
 */
FW_API std::string_view GetAppExecutablePath();

/**
 * 获取程序EXE/ELF文件所在目录
 */
FW_API std::string_view GetAppBinaryDirPath();

/**
 * 获取应用程序数据目录
 * Win32: %APPDATA%\<Exe文件名>\
 * *nix : ~/.cache/<elf文件名>
 */
FW_API std::string_view GetAppDataDirPath();

/**
 * 获取临时文件夹路径
 */
FW_API std::string_view GetTempDirPath();

/**
 * 获取临时文件夹下的一个路径
 */
FW_API std::string GetTempFilePath(const char* relPath);

/**
 * 获取程序资源目录下的资源
 */
FW_API std::string GetAssetsPath(const char* relPath);

/**
 * 获取AppData/<AppName>目录下的资源
 */
FW_API std::string GetDataPath(const char* relPath);

/**
 * 获取AppData/<AppName>目录下的资源
 */
FW_API std::string GetBinaryPath(const char* relPath);

FW_API std::string_view GetDesktopPath();

inline std::string operator""_AP(const char* relPath, size_t)
{
    return GetAssetsPath(relPath);
}

inline std::string operator""_DP(const char* relPath, size_t)
{
    return GetDataPath(relPath);
}

inline std::string operator""_TP(const char *relPath, size_t)
{
    return GetTempFilePath(relPath);
}

inline std::string operator""_BP(const char *relPath, size_t)
{
    return GetBinaryPath(relPath);
}

#endif //INFOSTATION_PATH_TOOLS_HPP
