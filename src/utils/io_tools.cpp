#include "utils/io_tools.hpp"
#include "core/log.hpp"
#include <core/assert.hpp>
#include "utils/AES.h"
#include <cstring>
#include <vector>

#if _WIN32

#include <windows.h>
#include "win32_utility.hpp"

static HANDLE OpenFileForRead(const std::string& path)
{
    auto const wpath = fromUtf8(path);
    HANDLE hFile = ::CreateFileW(wpath.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
        );
    LC_DEBUG(hFile == INVALID_HANDLE_VALUE, "file %s not found!", path.c_str());
    return hFile;
}

std::string ReadTextFile(const std::string& path)
{
    std::string text;
    if (auto hFile = OpenFileForRead(path); hFile != INVALID_HANDLE_VALUE) {
        LARGE_INTEGER move{ .QuadPart = 0 };
        LARGE_INTEGER pos{};
        FW_ASSERT(SetFilePointerEx(hFile, move, &pos, FILE_END) == TRUE);
        move.QuadPart = -pos.QuadPart;
        text.resize(pos.QuadPart);
        FW_ASSERT(SetFilePointerEx(hFile, move, &pos, FILE_CURRENT) == TRUE);
        DWORD readed;
        FW_ASSERT(ReadFile(hFile, &text[0], -move.QuadPart, &readed, NULL) == TRUE);
        text.resize(readed);
        CloseHandle(hFile);
    }
    else
    {
        L_WARN("read binary file %s error %d", path.c_str(), GetLastError());
    }
    return text;
}

std::vector<u8> ReadBinaryFile(const std::string& path)
{
    std::vector<u8> content;
    if (auto hFile = OpenFileForRead(path); hFile != INVALID_HANDLE_VALUE) {
        LARGE_INTEGER move{ .QuadPart = 0 };
        LARGE_INTEGER pos{};
        FW_ASSERT(SetFilePointerEx(hFile, move, &pos, FILE_END) == TRUE);
        move.QuadPart = -pos.QuadPart;
        content.resize(pos.QuadPart);
        FW_ASSERT(SetFilePointerEx(hFile, move, &pos, FILE_CURRENT) == TRUE);
        DWORD readed;
        FW_ASSERT(ReadFile(hFile, &content[0], -move.QuadPart, &readed, NULL) == TRUE);
        content.resize(readed);
        CloseHandle(hFile);
    }
    else
    {
        L_WARN("read binary file %s error %d", path.c_str(), GetLastError());
    }
    return content;
}

#else
#include <fcntl.h>
#include <unistd.h>

std::string ReadTextFile(const std::string& path)
{
    std::string text;
    int fd = open(path.c_str(), O_RDONLY);
    if (fd >= 0) {
        // get file length
        const auto length = lseek(fd, 0, SEEK_END);
        text.resize(length);
        lseek(fd,0,SEEK_SET);
        auto const r = read(fd, text.data(), length);
        (void)r;
        close(fd);
    }
    else {
        L_WARN("read text file %s error %s", path.c_str(), strerror (errno));
    }
    return text;
}

std::vector<u8> ReadBinaryFile (const std::string &path)
{
    std::vector<u8> content;
    int fd = open(path.c_str(), O_RDONLY);
    if (fd >= 0) {
        // get file length
        const auto length = lseek(fd, 0, SEEK_END);
        content.resize(length);
        lseek(fd,0,SEEK_SET);
        auto const r = read(fd, content.data(), length);
        (void)r;
        close(fd);
    }
    else {
        L_WARN("read binary file %s error %s", path.c_str(), strerror (errno));
    }
    return content;
}

#endif

std::vector<u8> EncryptContent(const void *dat, u32 size, const u8 (&key)[32])
{
    std::vector<u8> buffer;

    if (dat && size) {
        AES aes(AESKeyLength::AES_256);
        auto c = aes.EncryptECB((const u8*)dat, size, key);
        buffer.resize(size);
        memcpy(buffer.data(), c, size);
        delete [] c;
    }

    return buffer;
}   

std::vector<u8> ReadEncryptedFile(const std::string &path, const u8 (&key)[32])
{
    auto content = ReadBinaryFile(path);
    if (!content.empty()) {

        AES aes(AESKeyLength::AES_256);
        auto c = aes.DecryptECB(content.data(), content.size(), key);
        memcpy(content.data(), c, content.size());
        delete []c;
    }
    return content;
}
