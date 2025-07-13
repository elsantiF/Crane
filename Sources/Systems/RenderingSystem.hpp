#pragma once

#include "Core/Types.hpp"
#include "ISystem.hpp"

namespace Crane::Graphics {
  class IRenderer;
}

namespace Crane::Systems {
  class RenderingSystem : public ISystem {
  public:
    explicit RenderingSystem(Graphics::IRenderer *renderer) : m_Renderer(renderer) {}

    void Update(World::World &world, f64 deltaTime) override;

  private:
    Graphics::IRenderer *m_Renderer;
  };
}