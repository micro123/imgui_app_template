#ifndef IO_TOOLS_HPP
#define IO_TOOLS_HPP

#include "config.h"
#include <string>
#include <cstring>
#include <vector>

 std::string ReadTextFile(const std::string& path);

 std::vector<u8> ReadBinaryFile(const std::string &path);

 std::vector<u8> EncryptContent(const void *dat, u32 size, const u8 (&key)[32]);

inline std::vector<u8> EncryptContent(const void *dat, u32 size, const void *key_data) {
    u8 key[32] {};
    memcpy(key, key_data, sizeof(key));
    return EncryptContent(dat, size, key);
}

 std::vector<u8> ReadEncryptedFile(const std::string &path, const u8 (&key)[32]);

inline std::vector<u8> ReadEncryptedFile(const std::string &path, const void *key_data) {
    u8 key[32] {};
    memcpy(key, key_data, sizeof(key));
    return ReadEncryptedFile(path, key);
}

#endif // IO_TOOLS_HPP
