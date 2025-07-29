#pragma once

#include "Core/Systems/ISystem.hpp"

namespace Crane::Graphics {
  class RenderingSystem : public Core::Systems::IRenderSystem {
  public:
    RenderingSystem() = default;
    ~RenderingSystem() override = default;

    void Initialize([[maybe_unused]] Scene::World &world) override {};
    void Shutdown([[maybe_unused]] Scene::World &world) override {};

    void Render(Scene::World &world, Graphics::IRenderer &renderer) override;
  };
}