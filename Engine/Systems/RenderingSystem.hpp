#pragma once

#include "ISystem.hpp"

namespace Crane::Systems {
  class RenderingSystem : public IRenderSystem {
  public:
    RenderingSystem() = default;
    ~RenderingSystem() override = default;

    void Initialize([[maybe_unused]] World::World &world) override {};
    void Shutdown([[maybe_unused]] World::World &world) override {};

    void Render(World::World &world, Graphics::IRenderer &renderer) override;
  };
}