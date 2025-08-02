#include "RenderPipeline.hpp"
#include "Base/Profiler.hpp"
#include "Base/Types.hpp"
#include "Scene/Components/Renderable.hpp"
#include "Scene/Components/Transform.hpp"

namespace Crane::Graphics {
  void RenderPipeline::Render(Scene::World &world) {
    PROFILE_SCOPE();
    m_Renderer.BeginFrame();
    m_Renderer.Clear(Graphics::Colors::CLEAR_COLOR);
    RenderScene(world);
    m_Renderer.EndFrame();
  }

  void RenderPipeline::RenderImGui(Function<void()> imguiCallback) {
    m_Renderer.BeginImGuiFrame();
    if (imguiCallback) {
      imguiCallback();
    }
    m_Renderer.EndImGuiFrame();
  }

  void RenderPipeline::Present() {
    m_Renderer.Present();
  }

  void RenderPipeline::RenderScene(Scene::World &world) {
    PROFILE_SCOPE();
    auto &registry = world.GetRegistry();
    auto view = registry.view<Scene::Components::Transform, Scene::Components::Renderable>();

    for (auto entity : view) {
      const auto &[transform, renderable] = view.get<Scene::Components::Transform, Scene::Components::Renderable>(entity);

      m_Renderer.SetTexture(renderable.textureId);
      m_Renderer.SetMesh(renderable.meshId);
      m_Renderer.DrawRenderable(transform.transform);
    }
  }
}