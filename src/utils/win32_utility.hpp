#ifndef WIN32_UTILITY_HPP
#define WIN32_UTILITY_HPP

#if defined(_WIN32)

#include <Windows.h>
#include <string>

inline std::wstring fromUtf8(const std::string &input) {
    const int len = MultiByteToWideChar(CP_UTF8, 0, input.c_str(), -1, nullptr, 0);
    std::wstring out_wide_string;
    out_wide_string.resize(len + 1);
    out_wide_string[len] = 0;
    MultiByteToWideChar(CP_UTF8, 0, input.c_str(), -1, &out_wide_string[0], len);
    out_wide_string.resize(len - 1);
    return out_wide_string;
}

inline std::string toUtf8(const std::wstring &input) {
    int length = WideCharToMultiByte(CP_UTF8, 0, input.data(), -1, nullptr, 0, nullptr, nullptr);
    std::string out_string;
    out_string.resize(length + 1);
    length = WideCharToMultiByte(CP_UTF8, 0, input.data(), -1, &out_string[0], length, nullptr, nullptr);
    out_string[length] = 0;
    out_string.resize(length - 1);
    return out_string;
}

#else
#error "This file must use under windows platform!!"
#endif

#endif /* WIN32_UTILITY_HPP */
