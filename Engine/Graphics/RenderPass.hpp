#pragma once

namespace Crane::Scene {
  class World;
}

namespace Crane::Graphics {
  class IRenderer;
  class RenderPass {
  public:
    RenderPass(IRenderer &renderer) : m_Renderer(renderer) {}
    virtual ~RenderPass() = default;

    virtual void Begin() = 0;
    virtual void Execute(Scene::World &world) = 0;
    virtual void End() = 0;

  protected:
    IRenderer &m_Renderer;
  };
}