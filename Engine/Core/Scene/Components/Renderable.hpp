#pragma once

#include "Base/Types.hpp"

namespace Crane::Scene::Components {
  struct Renderable {
    Id meshId = 0;
    Id textureId = 0;

    Renderable(Id meshId = 0, Id textureId = 0) : meshId(meshId), textureId(textureId) {}
  };
}