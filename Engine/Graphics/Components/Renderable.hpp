#pragma once

#include "Base/Types.hpp"
#include "Graphics/Primitives/Mesh.hpp"

namespace Crane::Scene::Components {
  struct Renderable {
    Graphics::Mesh mesh;
    Id textureId;

    Renderable(Graphics::Mesh mesh, Id textureId) : mesh(mesh), textureId(textureId) {}
  };
}