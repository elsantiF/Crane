#pragma once

#include "Core/Types.hpp"
#include "Graphics/Color.hpp"

namespace Crane::Components {
  struct Renderable {
    Graphics::Color color;
    u32 vertexDataId = 0;
    u32 indexDataId = 0;
    u32 textureDataId = 0;

    Renderable(Graphics::Color color = Graphics::Colors::White, u32 vertexDataId = 0, u32 indexDataId = 0, u32 textureDataId = 0)
        : color(color), vertexDataId(vertexDataId), indexDataId(indexDataId), textureDataId(textureDataId) {}
  };
}