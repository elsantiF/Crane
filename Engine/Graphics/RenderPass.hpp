#pragma once

#include "Graphics/Renderer/IRenderer.hpp"
#include "Scene/World.hpp"

namespace Crane::Graphics {
  class RenderPass {
  public:
    RenderPass(IRenderer &renderer) : m_Renderer(renderer) {}
    virtual ~RenderPass() = default;

    virtual void Render(Scene::World &world) = 0;

  protected:
    IRenderer &m_Renderer;
  };
}