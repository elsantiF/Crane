#pragma once

#include "ISystem.hpp"

namespace Crane::Graphics {
  class IRenderer;
}

namespace Crane::Systems {
  class RenderingSystem : public IRenderSystem {
  public:
    RenderingSystem() = default;

    void Render(World::World &world, Graphics::IRenderer &renderer) override;
  };
}