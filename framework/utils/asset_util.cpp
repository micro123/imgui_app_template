#include "utils/asset_util.h"
#include "core/assert.hpp"
#include "utils/path_tools.hpp"

#ifdef _WIN32

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>

std::vector<unsigned char> asset_content(const char *asset_id)
{
    std::vector<unsigned char> content;
    std::string full_path = GetAssetsPath(asset_id);
    int fd = _open(full_path.data(), O_RDONLY);

    if (fd > 0)
    {
        size_t length = _lseeki64(fd, 0, SEEK_END);
        content.resize(length);
        _lseeki64(fd, 0, SEEK_SET);
        auto read_len = read(fd, content.data(), content.size());
        PI_ASSERT((size_t)read_len == content.size());
        close(fd);
    }
    else
        perror(full_path.data());

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
