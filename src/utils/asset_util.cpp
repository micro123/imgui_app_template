#include "utils/asset_util.h"
#include "core/assert.hpp"
#include "utils/path_tools.hpp"

#ifdef _WIN32

#include "win32_utility.hpp"

std::vector<unsigned char> asset_content(const char* asset_id)
{
    std::vector<unsigned char> content;
    std::wstring full_path_w = fromUtf8(GetAssetsPath(asset_id));

    HANDLE hFile = ::CreateFileW(full_path_w.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile != INVALID_HANDLE_VALUE)
    {
        LARGE_INTEGER size{};
        DWORD read_len{};
        FW_ASSERT(GetFileSizeEx(hFile, &size));
        content.resize(size.QuadPart);
        FW_ASSERT(ReadFile(hFile, content.data(), content.size(), &read_len, NULL));
        CloseHandle(hFile);
    }
    else
    {
        L_FATAL("Cannot open asset: ", asset_id);
    }

    return content;
}

#else
#include <fcntl.h>
#include <unistd.h>


std::vector<unsigned char> asset_content(const char *asset_id)
{
    std::vector<unsigned char> content;
    std::string full_path = GetAssetsPath(asset_id);
    int fd = open(full_path.data(), O_RDONLY);

    if (fd > 0)
    {
        size_t length = lseek64(fd, 0, SEEK_END);
        content.resize(length);
        lseek64(fd, 0, SEEK_SET);
        auto read_len = read(fd, content.data(), content.size());
        PI_ASSERT((size_t)read_len == content.size());
        close(fd);
    }
    else
        perror(full_path.data());

    return content;
}

#endif
