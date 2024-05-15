//
// Created by tang on 2021/11/16.
//

#ifndef INFO_CENTER_TEXTURE_H
#define INFO_CENTER_TEXTURE_H

#include "imgui.h"
#include <cstdint>

struct AssetsTexurePrivateData;

class IMGUI_API Texture
{
public:
    Texture ();

    ~Texture ();

    void LoadFromAsset(const char *assets_id);

    void LoadFromMem(const void *data, int len);

    void LoadFromFile(const char *path);

    int Width() const;

    int Height() const;

    intptr_t Id() const;

private:
    AssetsTexurePrivateData *d;
};

namespace ImGui {
    void IMGUI_API Image(const Texture &texture, ImVec2 scale = {1.f,1.f});
}

#endif //INFO_CENTER_TEXTURE_H
