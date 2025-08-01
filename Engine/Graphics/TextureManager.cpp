#include "Graphics/TextureManager.hpp"
#include "Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Crane::Graphics {
  Expected<Ref<Texture>, Resources::ResourceManagerError> TextureManager::LoadTextureFunc(const Path &path) {
    u32 width, height, channels;
    unsigned char *data = stbi_load(path.generic_string().c_str(), reinterpret_cast<int *>(&width), reinterpret_cast<int *>(&height),
                                    reinterpret_cast<int *>(&channels), 4);

    if (!data) {
      Logger::Error("TextureManager: Failed to load texture from path: {}", path.generic_string());
      return std::unexpected(Resources::ResourceManagerError::LoadFailed);
    }

    Vector<u32> pixelData;
    for (u32 i = 0; i < width * height; ++i) {
      u32 r = data[i * 4 + 0];
      u32 g = data[i * 4 + 1];
      u32 b = data[i * 4 + 2];
      u32 a = data[i * 4 + 3];
      pixelData.push_back((a << 24) | (b << 16) | (g << 8) | r);
    }

    stbi_image_free(data);
    return MakeRef<Texture>(width, height, std::move(pixelData));
  }
}
