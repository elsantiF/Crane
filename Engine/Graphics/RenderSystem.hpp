#pragma once

#include "Base/Math/Matrix.hpp"
#include "Core/Scene/World.hpp"
#include "Graphics/Primitives/Mesh.hpp"
#include "Graphics/Primitives/Texture.hpp"
#include "Graphics/Renderer/IRenderer.hpp"

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

    // Resource methods
    Mesh CreateMesh(const RawMesh &rawMesh);
    Id CreateTexture(const Texture &rawTexture);

  private:
    Scope<Graphics::IRenderer> m_Renderer;
    Id m_RenderPipelineId;
    Id m_SamplerId;

    struct MVP {
      Math::Mat4 model = Math::Mat4(1.0f);
      Math::Mat4 view = Math::Mat4(1.0f);
      Math::Mat4 projection = Math::Mat4(1.0f);
    } m_MVP;
  };
}