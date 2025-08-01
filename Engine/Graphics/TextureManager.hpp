#pragma once

#include "Base/Types.hpp"
#include "Core/Resources/ResourceManager.hpp"

namespace Crane::Graphics {
  struct Texture;
  class TextureManager : public Resources::ResourceManager<Texture> {
  public:
    TextureManager() {
      SetLoader(LoadTextureFunc);
    };

    Expected<Resources::Resource<Texture>, Resources::ResourceManagerError> LoadTexture(const Path &path) {
      return Load(path);
    };

  private:
    static Expected<Ref<Texture>, Resources::ResourceManagerError> LoadTextureFunc(const Path &path);
  };
}