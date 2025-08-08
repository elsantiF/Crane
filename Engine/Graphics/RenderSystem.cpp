#include "RenderSystem.hpp"
#include "Base/Profiler.hpp"
#include "Graphics/Primitives/Vertex.hpp"
#include "Graphics/Renderer/IRenderer.hpp"

namespace Crane::Graphics::Systems {

  RenderSystem::RenderSystem(Scope<Graphics::IRenderer> &renderer) : m_Renderer(std::move(renderer)) {
    PROFILE_SCOPE();
    m_Renderer->Initialize();
  }

  void RenderSystem::Render(Scene::World &world) {
    PROFILE_SCOPE();
    for (auto &renderPass : m_RenderPasses) {
      m_Renderer->BeginRenderPass();
      renderPass->Render(world);
      m_Renderer->EndRenderPass();
    }
  }

  void RenderSystem::RenderImGui(Function<void()> OnImGui) {
    PROFILE_SCOPE();
    m_Renderer->BeginImGuiFrame();
    OnImGui();
    m_Renderer->EndImGuiFrame();
  }

  Mesh RenderSystem::CreateMesh(const RawMesh &createInfo) {
    return Mesh{
        .vertexBufferId = m_Renderer->CreateBuffer<const SVertex2>(Graphics::BufferType::Vertex, createInfo.vertices),
        .indexBufferId = m_Renderer->CreateBuffer<const u32>(Graphics::BufferType::Index, createInfo.indices),
        .indexCount = static_cast<u32>(createInfo.indices.size()),
    };
  }

  Id RenderSystem::CreateTexture(const Texture &texture) {
    return m_Renderer->CreateTexture(texture);
  }
}