//
// Created by tang on 2021/11/16.
//

#include "texture.h"
#include "imgui/utils/glad.h"

#include "stb_image.h"

#include "utils/asset_util.h"
#include "utils/io_tools.hpp"

#include "core/log.hpp"

// Simple helper function to load an image into a OpenGL texture with common settings
static bool LoadTextureFromAssets(const stbi_uc* buffer, int len, GLuint* out_texture, int* out_width, int* out_height)
{
    // LoadFromAsset from file
    int image_width = 0;
    int image_height = 0;
    int channels;
    unsigned char* image_data = stbi_load_from_memory(buffer, len, &image_width, &image_height, &channels, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

struct AssetsTexurePrivateData {
    GLuint texture_id;
    int    texture_width;
    int    texture_height;
};

Texture::Texture () {
    d = new AssetsTexurePrivateData{0,0,0};
}

Texture::~Texture () {
    glDeleteTextures(1, &d->texture_id);
    delete d;
}

int Texture::Width() const {
    return d->texture_width;
}

int Texture::Height() const {
    return d->texture_height;
}

intptr_t Texture::Id() const {
    return d->texture_id;
}

void Texture::LoadFromAsset(const char *assets_id) {
    auto content = asset_content(assets_id);
    if (!LoadTextureFromAssets((const stbi_uc*)content.data(), content.size(), &d->texture_id, &d->texture_width, &d->texture_height))
    {
        L_ERROR("Failed to load Assets Texture: %s", assets_id);
    }
}

void Texture::LoadFromMem(const void *data, int len) {
    if (!LoadTextureFromAssets((const stbi_uc*)data, len, &d->texture_id, &d->texture_width, &d->texture_height))
    {
        L_ERROR("Failed to load Assets Texture!");
    }
}

void Texture::LoadFromFile (const char *path) {
    auto content = ReadBinaryFile (path);
    if (!LoadTextureFromAssets((const stbi_uc*)content.data(), content.size(), &d->texture_id, &d->texture_width, &d->texture_height))
    {
        L_ERROR("Failed to load Assets Texture!");
    }
}

#include "imgui.h"

namespace ImGui {
    void Image(const Texture &texture, ImVec2 scale) {
        Image((ImTextureID)texture.Id(), ImVec2(scale.x * (float)texture.Width(), scale.y * (float)texture.Height()));
    }
}
