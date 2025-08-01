#pragma once

#include "IRenderer.hpp"
#include "Scene/World.hpp"
#include "Systems/ISystem.hpp"

namespace Crane::Graphics {
  class RenderPipeline {
  public:
    RenderPipeline(IRenderer &renderer) : m_Renderer(renderer) {}

    void Render(Scene::World &world);
    void RenderImGui(Function<void()> imguiCallback);
    void Present();

  private:
    void RenderScene(Scene::World &world);

  private:
    IRenderer &m_Renderer;
  };
}