#include "RenderPipeline.hpp"
#include "Base/Profiler.hpp"
#include "Base/Types.hpp"

namespace Crane::Graphics {
  void RenderPipeline::Render(Scene::World &world) {
    PROFILE_SCOPE();
    m_Renderer->BeginFrame();
    RenderScene(world);
    m_Renderer->EndFrame();
  }

  void RenderPipeline::RenderImGui(Function<void()> imguiCallback) {
    m_Renderer->BeginImGuiFrame();
    if (imguiCallback) {
      imguiCallback();
    }
    m_Renderer->EndImGuiFrame();
  }

  void RenderPipeline::RenderScene(Scene::World &world) {
    PROFILE_SCOPE();
    for (auto &pass : m_RenderPasses) {
      pass->Begin();
      pass->Execute(world);
      pass->End();
    }
  }
}