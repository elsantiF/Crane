#include "RenderingSystem.hpp"
#include "Base/Profiler.hpp"
#include "Graphics/IRenderer.hpp"
#include "Scene/Components/Renderable.hpp"
#include "Scene/Components/Transform.hpp"
#include "Scene/World.hpp"

namespace Crane::Graphics {
  void RenderingSystem::Render(Scene::World &world, Graphics::IRenderer &renderer) {
    PROFILE_SCOPE();
    auto &registry = world.GetRegistry();
    auto view = registry.view<Scene::Components::Transform, Scene::Components::Renderable>();

    for (auto entity : view) {
      const auto &[transform, renderable] = view.get<Scene::Components::Transform, Scene::Components::Renderable>(entity);

      renderer.SetTexture(renderable.textureDataId);
      renderer.SetVertexData(renderable.vertexDataId);
      renderer.SetIndexData(renderable.indexDataId);
      renderer.DrawRenderable(transform.transform);
    }
  }
}