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

    Resources::Resource<Texture> LoadTexture(const Path &path) {
      return Load(path);
    };

  private:
    static Ref<Texture> LoadTextureFunc(const Path &path);
  };
}