#pragma once

#include "Base/Assert.hpp"
#include "IRenderer.hpp"
#include "RenderPass.hpp"
#include "Scene/World.hpp"
#include "Systems/ISystem.hpp"

namespace Crane::Graphics {
  class RenderPipeline {
  public:
    RenderPipeline(Scope<IRenderer> renderer) : m_Renderer(std::move(renderer)) {
      if (!m_Renderer) {
        Assert::Crash("Renderer cannot be null");
      }

      m_Renderer->Initialize();
    }

    void Render(Scene::World &world);
    void RenderImGui(Function<void()> imguiCallback);
    void Present();

    void AddRenderPass(Scope<RenderPass> pass) {
      m_RenderPasses.push_back(std::move(pass));
    }

    IRenderer &GetRenderer() {
      return *m_Renderer;
    }

  private:
    void RenderScene(Scene::World &world);

  private:
    Scope<IRenderer> m_Renderer;
    Vector<Scope<RenderPass>> m_RenderPasses;
  };
}