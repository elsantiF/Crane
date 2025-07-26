#pragma once

#include "Core/Types.hpp"

namespace Crane::Components {
  struct Renderable {
    u32 vertexDataId = 0;
    u32 indexDataId = 0;
    u32 textureDataId = 0;

    Renderable(u32 vertexDataId = 0, u32 indexDataId = 0, u32 textureDataId = 0)
        : vertexDataId(vertexDataId), indexDataId(indexDataId), textureDataId(textureDataId) {}
  };
}