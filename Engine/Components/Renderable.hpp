#pragma once

#include "Core/Types.hpp"

namespace Crane::Components {
  struct Renderable {
    Id vertexDataId = 0;
    Id indexDataId = 0;
    Id textureDataId = 0;

    Renderable(Id vertexDataId = 0, Id indexDataId = 0, Id textureDataId = 0)
        : vertexDataId(vertexDataId), indexDataId(indexDataId), textureDataId(textureDataId) {}
  };
}