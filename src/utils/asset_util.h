#ifndef ASSET_UTIL_H
#define ASSET_UTIL_H

#include "config.h"
#include "fw_export.h"
#include <vector>

FW_API std::vector<unsigned char> asset_content(const char *asset_id);

#endif /* ASSET_UTIL_H */
