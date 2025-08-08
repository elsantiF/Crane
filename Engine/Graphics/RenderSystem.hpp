#pragma once

#include "Core/Scene/World.hpp"
#include "Graphics/Primitives/Mesh.hpp"
#include "Graphics/Primitives/Texture.hpp"
#include "Graphics/Renderer/IRenderer.hpp"
#include "RenderPass.hpp"

namespace Crane::Graphics::Systems {
  class RenderSystem {
  public:
    RenderSystem(Scope<Graphics::IRenderer> &renderer);
    ~RenderSystem() = default;

    // Rendering methods
    void BeginFrame() {
      m_Renderer->BeginFrame();
    }

    void Render(Scene::World &world);
    void RenderImGui(Function<void()> OnImGui);

    void EndFrame() {
      m_Renderer->EndFrame();
    }

    // Render passes
    template <typename T, typename... Args>
    void AddRenderPass(Args &&...args) {
      m_RenderPasses.push_back(MakeScope<T>(*m_Renderer, std::forward<Args>(args)...));
    }

    // Resource methods
    Mesh CreateMesh(const RawMesh &rawMesh);
    Id CreateTexture(const Texture &rawTexture);

  private:
    Scope<Graphics::IRenderer> m_Renderer;
    Vector<Scope<RenderPass>> m_RenderPasses;
  };
}