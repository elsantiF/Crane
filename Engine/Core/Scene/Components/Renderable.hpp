#pragma once

#include "Base/Types.hpp"

namespace Crane::Scene::Components {
  struct Renderable {
    Id vertexBufferId = 0;
    Id indexBufferId = 0;
    Id textureId = 0;
    u32 indexCount = 0;

    Renderable(Id vertexBufferId = 0, Id indexBufferId = 0, Id textureId = 0, u32 indexCount = 0)
        : vertexBufferId(vertexBufferId), indexBufferId(indexBufferId), textureId(textureId), indexCount(indexCount) {}
  };
}