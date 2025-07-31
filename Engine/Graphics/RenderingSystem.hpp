#pragma once

#include "Core/Systems/ISystem.hpp"

namespace Crane::Graphics {
  class RenderingSystem : public Systems::IRenderSystem {
  public:
    RenderingSystem() = default;
    ~RenderingSystem() override = default;

    void Render(Scene::World &world, Graphics::IRenderer &renderer) override;
  };
}